#include "../../../Public/Shader/Basic/DefaultShader.h"

#include <codecvt>
#include <d3dcompiler.h>
#include <algorithm>

#include "../../../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/util.h"
#include "../../../Public/GameHost.h"
#include "../../../Public/Light/AmbiantLight.h"
#include "Api/Public/Util/FileWatcher.h"
#include "Api/Public/Util/Util.h"

// Definir l’organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC layout[] =
{
    {
        "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D11_INPUT_PER_VERTEX_DATA, 0
    },
    {
        "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
        D3D11_INPUT_PER_VERTEX_DATA, 0
    },
    {
        "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,
        D3D11_INPUT_PER_VERTEX_DATA, 0
    },
    {
        "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,
        D3D11_INPUT_PER_VERTEX_DATA, 0
    },
    {
        "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48,
        D3D11_INPUT_PER_VERTEX_DATA, 0
    }
};
UINT numElements;

PM3D_API::DefaultShader::DefaultShader(
    const std::wstring& fileName
) : Shader(),
    vertexBuffer(nullptr),
    indexBuffer(nullptr),
    fileWatcher(fileName, [fileName, this]()
    {
        std::lock_guard<std::mutex> guard{reloadingMutex};
        
        std::cout << "Reloading shader " << Util::ws2s(fileName) << std::endl;
        Destroy();

        Sleep(150); // To avoid issues
        
        Initialize(fileName);

        std::cout << "Shader reloaded!" << std::endl;
    })
{
    Initialize(fileName);
}

PM3D_API::DefaultShader::~DefaultShader()
{
    fileWatcher.~FileWatcher();

    Destroy();

    PM3D::DXRelacher(vertexBuffer);
    PM3D::DXRelacher(indexBuffer);
}

void PM3D_API::DefaultShader::Initialize(const std::wstring& wstring)
{
    const auto pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

    D3D11_BUFFER_DESC shaderParametersBufferDesc;
    ZeroMemory(&shaderParametersBufferDesc, sizeof(shaderParametersBufferDesc));

    shaderParametersBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    shaderParametersBufferDesc.ByteWidth = sizeof(DefaultShaderParameters);
    shaderParametersBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    shaderParametersBufferDesc.CPUAccessFlags = 0;

    PM3D::DXEssayer(pD3DDevice->CreateBuffer(&shaderParametersBufferDesc, nullptr, &shaderParametersBuffer));

    // Convert wstring to string
    // converter
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string convertedFileName = converter.to_bytes(wstring);

    ID3DBlob* pFXBlob = nullptr;

    ID3DBlob* errorBlob = nullptr;
    auto res = D3DCompileFromFile(
        wstring.c_str(),
        0,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        0,
        "fx_5_0",
        D3DCOMPILE_DEBUG,
        0,
        &pFXBlob,
        &errorBlob
    );

    if (res != S_OK)
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        throw std::runtime_error("Shader compilation failed! (DefaultShader > " + convertedFileName + ")");
    }

    res = D3DX11CreateEffectFromMemory(
        pFXBlob->GetBufferPointer(),
        pFXBlob->GetBufferSize(),
        D3DCOMPILE_DEBUG,
        pD3DDevice,
        &effect
    );

    if (res != S_OK)
    {
        throw std::runtime_error("Shader creation failed! (DefaultShader > " + convertedFileName + ")");
    }

    pFXBlob->Release();

    if (effect == nullptr)
    {
        throw std::runtime_error("Effect not initialized!");
    }

    technique = effect->GetTechniqueByName("ShadowMap");
    passe = technique->GetPassByIndex(0);

    {
        D3DX11_PASS_SHADER_DESC effectVSDesc;
        PM3D::DXEssayer(passe->GetVertexShaderDesc(&effectVSDesc));

        D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
        PM3D::DXEssayer(effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2));

        const void* vsCodePtr = effectVSDesc2.pBytecode;
        const uint32_t vsCodeLen = effectVSDesc2.BytecodeLength;
        vertexLayout = nullptr;

        numElements = ARRAYSIZE(layout);

        PM3D::DXEssayer(pD3DDevice->CreateInputLayout(layout,
                                                      numElements,
                                                      vsCodePtr,
                                                      vsCodeLen,
                                                      &vertexLayoutShadow),
                        DXE_CREATIONLAYOUT);
    }

    technique = effect->GetTechniqueByIndex(0);
    passe = technique->GetPassByIndex(0);

    D3DX11_PASS_SHADER_DESC effectVSDesc;
    passe->GetVertexShaderDesc(&effectVSDesc);

    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

    const void* vsCodePtr = effectVSDesc2.pBytecode;
    const uint32_t vsCodeLen = effectVSDesc2.BytecodeLength;
    vertexLayout = nullptr;

    numElements = ARRAYSIZE(layout);

    PM3D::DXEssayer(pD3DDevice->CreateInputLayout(layout,
                                                  numElements,
                                                  vsCodePtr,
                                                  vsCodeLen,
                                                  &vertexLayout),
                    DXE_CREATIONLAYOUT);

    // Initialisation des paramètres de sampling de la texture
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 4;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Création de l’état de sampling
    pD3DDevice->CreateSamplerState(&samplerDesc, &albedoSampleState);
    pD3DDevice->CreateSamplerState(&samplerDesc, &normalmapSampleState);

    D3D11_TEXTURE2D_DESC depthTextureDesc;
    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
    depthTextureDesc.Width = 512;
    depthTextureDesc.Height = 512;
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthTextureDesc.SampleDesc.Count = 1;
    depthTextureDesc.SampleDesc.Quality = 0;
    depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
    depthTextureDesc.CPUAccessFlags = 0;
    depthTextureDesc.MiscFlags = 0;

    PM3D::DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture), DXE_ERREURCREATIONTEXTURE);

    // Création de la vue du tampon de profondeur (ou de stencil)
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
    ZeroMemory(&descDSView, sizeof(descDSView));
    descDSView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSView.Texture2D.MipSlice = 0;

    PM3D::DXEssayer(pD3DDevice->CreateDepthStencilView(depthTexture, &descDSView, &depthStencilView),
                    DXE_ERREURCREATIONDEPTHSTENCILTARGET);

    // Création d’une shader resource view pour lire le tampond de profondeur dans le shader.
    D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
    sr_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    sr_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    sr_desc.Texture2D.MostDetailedMip = 0;
    sr_desc.Texture2D.MipLevels = 1;
    PM3D::DXEssayer(pD3DDevice->CreateShaderResourceView(depthTexture, &sr_desc, &depthShaderResourceView));
}

void PM3D_API::DefaultShader::Destroy()
{
    PM3D::DXRelacher(shaderParametersBuffer);
    PM3D::DXRelacher(effect);
    PM3D::DXRelacher(vertexLayout);
    PM3D::DXRelacher(albedoSampleState);
    PM3D::DXRelacher(normalmapSampleState);
    PM3D::DXRelacher(depthStencilView);
    PM3D::DXRelacher(depthShaderResourceView);
    PM3D::DXRelacher(depthTexture);
    PM3D::DXRelacher(vertexLayoutShadow);
}

ID3D11Buffer* PM3D_API::DefaultShader::GetShaderParametersBuffer() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return shaderParametersBuffer;
}

ID3DX11EffectPass* PM3D_API::DefaultShader::GetPass() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return passe;
}

ID3D11InputLayout* PM3D_API::DefaultShader::GetVertexLayout() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return vertexLayout;
}

ID3D11Buffer* PM3D_API::DefaultShader::GetIndexBuffer() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return indexBuffer;
}

ID3D11InputLayout* PM3D_API::DefaultShader::GetShadowVertexLayout() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return vertexLayoutShadow;
}

ID3D11Texture2D* PM3D_API::DefaultShader::GetDepthTexture() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return depthTexture;
}

ID3D11DepthStencilView* PM3D_API::DefaultShader::GetDepthStencilView() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return depthStencilView;
}

ID3D11ShaderResourceView* PM3D_API::DefaultShader::GetDepthShaderResourceView() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return depthShaderResourceView;
}

ID3DX11Effect* PM3D_API::DefaultShader::GetEffect() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return effect;
}

ID3D11Buffer** PM3D_API::DefaultShader::GetVertexBufferPtr() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return &vertexBuffer;
}

ID3D11Buffer** PM3D_API::DefaultShader::GetIndexBufferPtr() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};
    return &indexBuffer;
}

void* PM3D_API::DefaultShader::PrepareParameters(
    DirectX::XMMATRIX matWorldViewProj,
    DirectX::XMMATRIX matWorld
)
{
    std::lock_guard<std::mutex> guard{reloadingMutex};

    ID3DX11EffectSamplerVariable* variableSampler = effect->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, albedoSampleState);

    ID3DX11EffectSamplerVariable* variableSamplerNM = effect->GetVariableByName("SampleStateNormalMap")->AsSampler();
    variableSamplerNM->SetSampler(0, normalmapSampleState);

    const auto cameraPos = GameHost::GetInstance()->GetScene()->GetMainCamera()->GetWorldPosition();
    const auto cameraDir = GameHost::GetInstance()->GetScene()->GetMainCamera()->GetWorldDirection();

    const auto parameters = new DefaultShaderParameters{
        matWorldViewProj,
        matWorld,
        DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f),
        DirectX::XMVectorSet(cameraDir.x, cameraDir.y, cameraDir.z, 1.0f)
    };

    return parameters;
}

void PM3D_API::DefaultShader::ApplyMaterialParameters(
    void* shaderParameters,
    DirectX::XMVECTOR materialAmbiant,
    DirectX::XMVECTOR materialDiffuse,
    DirectX::XMVECTOR materialSpecular,
    float specularPower,
    ID3D11ShaderResourceView* albedoTexture,
    ID3D11ShaderResourceView* normalmapTexture
)
{
    std::lock_guard<std::mutex> guard{reloadingMutex};

    auto& parameters = *static_cast<DefaultShaderParameters*>(shaderParameters);

    parameters.materialAmbiant = materialAmbiant;
    parameters.materialDiffuse = materialDiffuse;
    parameters.materialSpecular = materialSpecular;
    parameters.materialSpecularPower = specularPower;

    if (albedoTexture == nullptr)
    {
        parameters.hasAlbedoTexture = false;
    }
    else
    {
        parameters.hasAlbedoTexture = true;

        ID3DX11EffectShaderResourceVariable* variableTexture = effect->GetVariableByName("textureEntree")->
                                                                       AsShaderResource();
        variableTexture->SetResource(albedoTexture);
    }

    if (normalmapTexture != nullptr)
    {
        const auto effectVariablePtr = effect->GetVariableByName("normalMap");

        if (effectVariablePtr)
        {
            ID3DX11EffectShaderResourceVariable* variableTexture = effectVariablePtr->AsShaderResource();
            variableTexture->SetResource(normalmapTexture);

            parameters.hasNormalmapTexture = true;
        }
    }

    if (depthShaderResourceView)
    {
        ID3DX11EffectShaderResourceVariable* pShadowMap = effect->GetVariableByName("shadowTexture")->
                                                                  AsShaderResource();
        pShadowMap->SetResource(depthShaderResourceView);
    }
}

void PM3D_API::DefaultShader::DeleteParameters(void* shader_parameters)
{
    std::lock_guard<std::mutex> guard{reloadingMutex};

    delete static_cast<DefaultShaderParameters*>(shader_parameters);
}

void PM3D_API::DefaultShader::ApplyShaderParams() const
{
    std::lock_guard<std::mutex> guard{reloadingMutex};

    ID3DX11EffectConstantBuffer* pCB = effect->GetConstantBufferByName("param");
    pCB->SetConstantBuffer(shaderParametersBuffer);
}

void PM3D_API::DefaultShader::LoadLights(ID3D11DeviceContext* context, GameObject* gameObject)
{
    std::lock_guard<std::mutex> guard{reloadingMutex};

    /*if (!GameHost::GetInstance()->GetScene()->GetLightsNeedUpdate())
    {
        return;
    }*/

    const auto& lights = GameHost::GetInstance()->GetScene()->GetLights();
    const auto lightCount = lights.size();

    auto finalLights = std::vector<Light*>{};

    if (lightCount > static_cast<std::vector<std::unique_ptr<Light>>::size_type>(10))
    {
        std::vector<Light*> ambiantLights{};
        std::vector<Light*> directionalLights{};
        std::vector<Light*> otherLights{};

        // Trop de lumières, on prend 1. Ambiant puis 2. Directional puis 3 par distance

        for (auto& light : lights)
        {
            if (light->GetType() == LightType::AMBIANT)
            {
                ambiantLights.push_back(light);
            }
            else if (light->GetType() == LightType::DIRECTIONAL)
            {
                directionalLights.push_back(light);
            }
            else
            {
                // TODO: Sort by distance
                otherLights.push_back(light);
            }
        }

        if (ambiantLights.size() > 10) // ... ????
        {
            // Copy the first 10 ambiant lights
            std::copy_n(ambiantLights.begin(), 10, finalLights.begin());
        }
        else
        {
            int remaining = 10 - static_cast<int>(ambiantLights.size());

            if (directionalLights.size() > remaining)
            {
                // Adds only the first remaining directional lights
                std::copy_n(directionalLights.begin(), remaining, finalLights.begin() + ambiantLights.size());
            }
            else
            {
                remaining -= static_cast<int>(directionalLights.size());

                // Adds the remaining lights
                std::copy_n(otherLights.begin(), remaining,
                            finalLights.begin() + ambiantLights.size() + directionalLights.size());
            }
        }
    }
    else
    {
        std::for_each(lights.begin(), lights.end(), [&finalLights](Light* light)
        {
            finalLights.push_back(light);
        });
    }

    std::vector<ShaderLightDefaultParameters> shaderLightsParameters{};
    std::for_each(finalLights.begin(), finalLights.end(), [&shaderLightsParameters, &gameObject](const Light* light)
    {
        shaderLightsParameters.push_back(light->GetShaderLightDefaultParameters(gameObject));
    });

    for (int i = 0; i < 10 - lightCount; ++i)
    {
        shaderLightsParameters.push_back(DefaultShaderLightDefaultParameters);
    }

    D3D11_BUFFER_DESC lightParametersBufferDesc;
    ZeroMemory(&lightParametersBufferDesc, sizeof(lightParametersBufferDesc));
    lightParametersBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightParametersBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    lightParametersBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    lightParametersBufferDesc.StructureByteStride = sizeof(ShaderLightDefaultParameters);
    lightParametersBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightParametersBufferDesc.ByteWidth = sizeof(ShaderLightDefaultParameters) * static_cast<UINT>(shaderLightsParameters.size());

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = shaderLightsParameters.data();

    ID3D11Buffer* lightBuffer = nullptr;
    const auto pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();
    PM3D::DXEssayer(pD3DDevice->CreateBuffer(&lightParametersBufferDesc, &initData, &lightBuffer));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.ElementWidth = static_cast<UINT>(shaderLightsParameters.size());

    ID3D11ShaderResourceView* lightSRV = nullptr;
    PM3D::DXEssayer(pD3DDevice->CreateShaderResourceView(lightBuffer, &srvDesc, &lightSRV));

    ID3DX11EffectShaderResourceVariable* variableTexture;
    variableTexture = effect->GetVariableByName("lights")->AsShaderResource();
    variableTexture->SetResource(lightSRV);
}
