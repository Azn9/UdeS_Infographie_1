#include "Core/Public/Object/PanneauPE.h"
#include "Core/Public/Util/resource.h"
#include "Core/Public/Core/dispositifD3D11.h"
#include <d3dcompiler.h>
#include <iostream>

#include "Core/Public/Object/Panneau.h"

namespace PM3D
{
    // Definir l’organisation de notre sommet
    D3D11_INPUT_ELEMENT_DESC CSommetPanneauPE::layout[] =
    {
        {"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 
         0,D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT, 
         0,12,D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    
    UINT CSommetPanneauPE::numElements = ARRAYSIZE(layout);
    
    CSommetPanneauPE CPanneauPE::sommets[3] = {
        CSommetPanneauPE(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        CSommetPanneauPE(XMFLOAT3(-1.0f, 3.0f, 0.0f), XMFLOAT2(0.0f, -1.0f)),
        CSommetPanneauPE(XMFLOAT3( 3.0f, -1.0f, 0.0f), XMFLOAT2(2.0f, 1.0f))
        };


    // FONCTION : CPanneauPE, constructeur paramètré 
    // BUT : Constructeur d’une classe de PanneauPE
    // PARAMÈTRE:
    // pDispositif: pointeur sur notre objet dispositif
    CPanneauPE::CPanneauPE(CDispositifD3D11* pDispositif_in)
    : pDispositif(pDispositif_in)
    , pVertexBuffer(nullptr)
    , pEffet(nullptr)
    , pTechnique(nullptr)
    , pPasse(nullptr)
    , pVertexLayout{nullptr}
    , pSampleState(nullptr)
    {
        // Création du vertex buffer et copie des sommets
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(sommets);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = sommets;
        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer),
        DXE_CREATIONVERTEXBUFFER);
        // Initialisation de l’effet 
        InitEffet(); 
    }

    void CPanneauPE::enableAllPostEffects()
    {
        int count = pEffectDesc->Techniques;
        for (int i = 0; i < count; ++i)
        {
            pEnabledTechniques.insert(i);       
        }
    }

    std::set<int>& CPanneauPE::getEnabledPostEffects()
    {
        return pEnabledTechniques;
    }

    const D3DX11_EFFECT_DESC* CPanneauPE::getEffectDesc() const
    {
        return pEffectDesc;
    }

    void CPanneauPE::Resize(const unsigned& width, const unsigned& height)
    {
        DXRelacher(pTmpResourceView);
        DXRelacher(pTmp2ResourceView);
        DXRelacher(pTmpRenderTargetView);
        DXRelacher(pTmp2RenderTargetView);
        DXRelacher(pTmpTexture);
        DXRelacher(pTmp2Texture);
        
        pTextureDesc.Width = width;
        pTextureDesc.Height = height;

        CreateTexturesAndViews();
    }

    void CPanneauPE::CreateDescriptions()
    {
        // Description de la texture
        ZeroMemory(&pTextureDesc, sizeof(pTextureDesc));
        // Cette texture sera utilisée comme cible de rendu et
        // comme ressource de shader
        pTextureDesc.Width = pDispositif->GetLargeur();
        pTextureDesc.Height = pDispositif->GetHauteur();
        pTextureDesc.MipLevels = 1;
        pTextureDesc.ArraySize = 1;
        pTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        pTextureDesc.SampleDesc.Count = 1;
        pTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        pTextureDesc.BindFlags =
            D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
        pTextureDesc.CPUAccessFlags = 0;
        pTextureDesc.MiscFlags = 0;
        
        // VUE - Cible de rendu
        pRenderTargetViewDesc.Format = pTextureDesc.Format;
        pRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        pRenderTargetViewDesc.Texture2D.MipSlice = 0;
        
        // VUE – Ressource de shader
        pShaderResourceViewDesc.Format = pTextureDesc.Format;
        pShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        pShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        pShaderResourceViewDesc.Texture2D.MipLevels = 1;
    }

    void CPanneauPE::CreateTexturesAndViews()
    {
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
        
        // Création de la texture 1
        pD3DDevice->CreateTexture2D(&pTextureDesc, nullptr, & pTmpTexture);

        // Création de la texture 2
        pD3DDevice->CreateTexture2D(&pTextureDesc, nullptr, & pTmp2Texture);

        // Création de la RTV 1
        pD3DDevice->CreateRenderTargetView(pTmpTexture,
                                           &pRenderTargetViewDesc,
                                           &pTmpRenderTargetView);
        
        // Création de la RTV 2
        pD3DDevice->CreateRenderTargetView(pTmp2Texture,
                                           &pRenderTargetViewDesc,
                                           &pTmp2RenderTargetView);
        
        // Création de la SRV 1
        pD3DDevice->CreateShaderResourceView(pTmpTexture, &pShaderResourceViewDesc, &pTmpResourceView);
        
        // Création de la SRV 2
        pD3DDevice->CreateShaderResourceView(pTmp2Texture, &pShaderResourceViewDesc, &pTmp2ResourceView);
    }

    void CPanneauPE::InitEffet()
    {
        // Compilation et chargement du vertex shader
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
        
        // Pour l’effet
        ID3DBlob* pFXBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;
        HRESULT result = D3DCompileFromFile( L"shader/PostEffect.fx", 0, 0, 0, 
        "fx_5_0", 0, 0, 
        &pFXBlob, &errorBlob);

        if (FAILED(result))
        {
            if (errorBlob)
            {
                char* errorMessage = static_cast<char*>(errorBlob->GetBufferPointer());
                std::cerr << "Shader compilation failed with errors:\n" << errorMessage << std::endl;
                errorBlob->Release();
            }
        }

        DXEssayer(result, DXE_ERREURCREATION_FX);
        D3DX11CreateEffectFromMemory( 
         pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, 
        pD3DDevice, &pEffet);
        pFXBlob->Release();
        
        pTechnique = pEffet->GetTechniqueByIndex(0); //On suppose qu'on aurait le même vs à chaque technique (NulVS)
        pPasse = pTechnique->GetPassByIndex(0);
        
        // Créer l’organisation des sommets pour le VS de notre effet
        D3DX11_PASS_SHADER_DESC effectVSDesc;
        D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
        pPasse->GetVertexShaderDesc(&effectVSDesc);
        effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, 
        &effectVSDesc2);
        const void *vsCodePtr = effectVSDesc2.pBytecode;
        unsigned vsCodeLen = effectVSDesc2.BytecodeLength;
        pVertexLayout = nullptr;
        DXEssayer( pD3DDevice->CreateInputLayout( CSommetPanneauPE::layout, 
        CSommetPanneauPE::numElements, 
        vsCodePtr, 
        vsCodeLen, 
        &pVertexLayout ),
        DXE_CREATIONLAYOUT);
        
        // Initialisation des paramètres de sampling de la texture
        // Pas nécessaire d’être compliqué puisque l’affichage sera
        // en 1 pour 1 et à plat
        D3D11_SAMPLER_DESC samplerDesc;
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; 
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 0;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        // Création de l’état de sampling
        pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);
        

        // *********************** VUES POUR LE POST EFFECT **************************
        
        CreateDescriptions();

        CreateTexturesAndViews();

        pEffectDesc = new D3DX11_EFFECT_DESC{};
        pEffet->GetDesc(pEffectDesc);

        // Le sampler state
        ID3DX11EffectSamplerVariable* variableSampler;
        variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
        variableSampler->SetSampler(0, pSampleState);
    }
    
    CPanneauPE::~CPanneauPE()
    {
        DXRelacher(pSampleState);
        DXRelacher(pEffet);
        DXRelacher(pVertexLayout);
        DXRelacher(pVertexBuffer);
        
        DXRelacher(pTmpResourceView);
        DXRelacher(pTmp2ResourceView);
        DXRelacher(pTmpRenderTargetView);
        DXRelacher(pTmp2RenderTargetView);
        DXRelacher(pTmpTexture);
        DXRelacher(pTmp2Texture);
    }

    void CPanneauPE::Draw()
    {
        if(pEnabledTechniques.empty())
            return;

        ID3DX11EffectShaderResourceVariable* variableTexture;
        variableTexture = pEffet->GetVariableByName("depthTexture")->AsShaderResource();
        variableTexture->SetResource(pMainDepthStencilShaderRessourceView);
        
        ID3D11DepthStencilState* oldDepthState = pDispositif->GetDepthStencilState();
        pDispositif->SetDepthState(false, false); //on désactive les tests et l'écriture sur la profondeur

        // Obtenir le contexte
        ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
        // Choisir la topologie des primitives
        pImmediateContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        // Source des sommets
        UINT stride = sizeof( CSommetPanneauPE );
        UINT offset = 0;
        pImmediateContext->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride,
        &offset );

        // input layout des sommets
        pImmediateContext->IASetInputLayout( pVertexLayout );

        int i{0};
        for (int techniqueIndex : pEnabledTechniques)
        {
            pCurrentResourceView = (i % 2 == 0) ? pTmpResourceView : pTmp2ResourceView;
            pCurrentRenderTargetView =
                (i == pEnabledTechniques.size() - 1) ? pMainRenderTargetView : //C'est la dérnière passe, on écrit sur la swap
                (i % 2 == 0) ? pTmp2RenderTargetView : pTmpRenderTargetView;

            pDispositif->SetRenderTargetView(pCurrentRenderTargetView, nullptr);
            
            // Choix de la technique
            pTechnique = pEffet->GetTechniqueByIndex(techniqueIndex);
            pPasse = pTechnique->GetPassByIndex(0);

            // La texture de la scène
            ID3DX11EffectShaderResourceVariable* variableTexture;
            variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
            variableTexture->SetResource(pCurrentResourceView);
        
            pPasse->Apply(0, pImmediateContext);
            // **** Rendu de l’objet
            pImmediateContext->Draw( 3, 0 );
            
            ++i;
        }
    }

    void CPanneauPE::BeginDrawToPostEffect()
    {
        if(pEnabledTechniques.empty())
            return;
        
        // Prendre en note l’ancienne surface de rendu
        pMainRenderTargetView = pDispositif->GetRenderTargetView();
        pMainDepthStencilView = pDispositif->GetDepthStencilView();
        pMainDepthStencilShaderRessourceView = pDispositif->GetDepthShaderRessourceView();
        // Utiliser la texture comme surface de rendu et le tampon de profondeur
        // associé
        pDispositif->SetRenderTargetView(pTmpRenderTargetView, pMainDepthStencilView);
    }

    void CPanneauPE::EndDrawToPostEffect()
    {
        if(pEnabledTechniques.empty())
            return;

        ID3DX11EffectShaderResourceVariable* variableTexture;
        variableTexture = pEffet->GetVariableByName("depthTexture")->AsShaderResource();
        variableTexture->SetResource(nullptr);

        pDispositif->SetRenderTargetView(pMainRenderTargetView, pMainDepthStencilView);
        pDispositif->SetDepthState(true, true);
    }

    HRESULT CPanneauPE::SetShaderVar(const std::string& name, const float& f) const
    {
        ID3DX11EffectScalarVariable* var = pEffet->GetVariableByName(name.c_str())->AsScalar();
        return var->SetFloat(f);
    }

    HRESULT CPanneauPE::SetShaderVar(const std::string& name, const int& i) const
    {
        ID3DX11EffectScalarVariable* var = pEffet->GetVariableByName(name.c_str())->AsScalar();
        return var->SetInt(i);
    }

    HRESULT CPanneauPE::SetShaderVar(const std::string& name, const XMFLOAT3& fs) const
    {
        ID3DX11EffectVectorVariable* var = pEffet->GetVariableByName(name.c_str())->AsVector();
        float fs2[3] = {fs.x, fs.y, fs.z};
        return var->SetFloatVector(fs2);
    }
    HRESULT CPanneauPE::SetShaderVar(const std::string& name, const XMFLOAT4& fs) const
    {
        ID3DX11EffectVectorVariable* var = pEffet->GetVariableByName(name.c_str())->AsVector();
        const float fs2[4] = {fs.x, fs.y, fs.z, fs.w};
        return var->SetFloatVector(fs2);
    }
    HRESULT CPanneauPE::SetShaderVar(const std::string& name, const XMVECTOR& fs) const
    {
        ID3DX11EffectVectorVariable* var = pEffet->GetVariableByName(name.c_str())->AsVector();
        return var->SetFloatVector(fs.m128_f32);
    }
    HRESULT CPanneauPE::SetShaderVar(const std::string& name, ID3D11SamplerState* s) const
    {
        ID3DX11EffectSamplerVariable* var = pEffet->GetVariableByName(name.c_str())->AsSampler();
        return var->SetSampler(0, s);
    }
    HRESULT CPanneauPE::SetShaderVar(const std::string& name, ID3D11ShaderResourceView* s) const
    {
        ID3DX11EffectShaderResourceVariable* var = pEffet->GetVariableByName(name.c_str())->AsShaderResource();
        return var->SetResource(s);
    }
    HRESULT CPanneauPE::SetShaderVar(const std::string& name, const XMMATRIX& m) const
    {
        ID3DX11EffectMatrixVariable* var = pEffet->GetVariableByName(name.c_str())->AsMatrix();
        XMFLOAT4X4 mat;
        XMStoreFloat4x4(&mat, m);
        return var->SetMatrix(&mat._11);
    }
}

