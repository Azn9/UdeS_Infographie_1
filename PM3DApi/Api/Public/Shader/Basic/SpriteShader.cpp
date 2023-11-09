#include "SpriteShader.h"

#include <codecvt>
#include <locale>

#include "../../../../../PetitMoteur3D/Core/Public/Sprite/AfficheurSprite.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/util.h"

// Definir l’organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC spriteShaderLayout[] =
{
    {
        "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D11_INPUT_PER_VERTEX_DATA, 0
    },
    {
        "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48,
        D3D11_INPUT_PER_VERTEX_DATA, 0
    }
};
UINT spriteShaderNumElements;

PM3D::CSommetSprite sommets[6] = {
    PM3D::CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
    PM3D::CSommetSprite(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
    PM3D::CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
    PM3D::CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
    PM3D::CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
    PM3D::CSommetSprite(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))
};

PM3D_API::SpriteShader::SpriteShader(const std::wstring& fileName) : Shader(), vertexBuffer(nullptr),
                                                                     indexBuffer(nullptr)
{
    const auto pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(sommets);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = sommets;

    PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &vertexBuffer), DXE_CREATIONVERTEXBUFFER);

    D3D11_BUFFER_DESC shaderParametersBufferDesc;
    ZeroMemory(&shaderParametersBufferDesc, sizeof(shaderParametersBufferDesc));

    shaderParametersBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    shaderParametersBufferDesc.ByteWidth = sizeof(SpriteShaderParameters);
    shaderParametersBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    shaderParametersBufferDesc.CPUAccessFlags = 0;

    PM3D::DXEssayer(pD3DDevice->CreateBuffer(&shaderParametersBufferDesc, nullptr, &shaderParametersBuffer));

    // Convert wstring to string
    // converter
#pragma warning(push)
#pragma warning(disable:4996)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string convertedFileName = converter.to_bytes(fileName);
#pragma warning(pop)

    ID3DBlob* pFXBlob = nullptr;

    ID3DBlob* errorBlob;
    auto res = D3DCompileFromFile(
        fileName.c_str(),
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
            OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
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

    technique = effect->GetTechniqueByIndex(0);
    passe = technique->GetPassByIndex(0);

    D3DX11_PASS_SHADER_DESC effectVSDesc;
    passe->GetVertexShaderDesc(&effectVSDesc);

    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

    const void* vsCodePtr = effectVSDesc2.pBytecode;
    const uint32_t vsCodeLen = effectVSDesc2.BytecodeLength;
    vertexLayout = nullptr;

    spriteShaderNumElements = ARRAYSIZE(spriteShaderLayout);

    PM3D::DXEssayer(pD3DDevice->CreateInputLayout(
                        spriteShaderLayout,
                        spriteShaderNumElements,
                        vsCodePtr,
                        vsCodeLen,
                        &vertexLayout),
                    DXE_CREATIONLAYOUT);

    // Initialisation des paramètres de sampling de la texture
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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
}

PM3D_API::SpriteShader::~SpriteShader()
{
    PM3D::DXRelacher(shaderParametersBuffer);
    PM3D::DXRelacher(effect);
    PM3D::DXRelacher(vertexLayout);
    PM3D::DXRelacher(vertexBuffer);
    PM3D::DXRelacher(indexBuffer);
    PM3D::DXRelacher(albedoSampleState);
}

void PM3D_API::SpriteShader::LoadLights(ID3D11DeviceContext* context, GameObject* gameObject)
{
    // No lights for sprites
}

void* PM3D_API::SpriteShader::PrepareParameters(DirectX::XMMATRIX matWorldViewProj, DirectX::XMMATRIX matWorld)
{
    ID3DX11EffectSamplerVariable* variableSampler = effect->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, albedoSampleState);

    const auto parameters = new SpriteShaderParameters{
        matWorldViewProj
    };

    return parameters;
}

void PM3D_API::SpriteShader::ApplyMaterialParameters(
    void* shaderParameters,
    DirectX::XMVECTOR materialAmbiant,
    DirectX::XMVECTOR materialDiffuse,
    DirectX::XMVECTOR materialSpecular,
    float specularPower,
    ID3D11ShaderResourceView* albedoTexture,
    ID3D11ShaderResourceView* normalmapTexture)
{
    if (albedoTexture == nullptr)
    {
        throw std::runtime_error("Trying to use SpriteShader without a texture!");
    }

    const auto variableTexture = effect->GetVariableByName("textureEntree")->AsShaderResource();
    variableTexture->SetResource(albedoTexture);
}

void PM3D_API::SpriteShader::DeleteParameters(void* shader_parameters)
{
    delete static_cast<SpriteShaderParameters*>(shader_parameters);
}
