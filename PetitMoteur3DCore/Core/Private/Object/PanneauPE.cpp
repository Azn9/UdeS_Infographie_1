#include "Core/Public/Object/PanneauPE.h"
#include "Core/Public/Util/util.h"
#include "Core/Public/Util/resource.h"
#include "Core/Public/Core/dispositifD3D11.h"
#include <d3dcompiler.h>

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
    
    CSommetPanneauPE CPanneauPE::sommets[6] = {
        CSommetPanneauPE(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        CSommetPanneauPE(XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
        CSommetPanneauPE(XMFLOAT3( 1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
        CSommetPanneauPE(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
        CSommetPanneauPE(XMFLOAT3( 1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
        CSommetPanneauPE(XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)) 
        };
    
    CPanneauPE::~CPanneauPE()
    {
        DXRelacher(pSampleState);
        DXRelacher(pEffet);
        DXRelacher(pVertexLayout);
        DXRelacher(pVertexBuffer);
    }

    void CPanneauPE::Draw()
    {
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
        // Choix de la technique
        pTechnique = pEffet->GetTechniqueByIndex(0);
        pPasse = pTechnique->GetPassByIndex(0);
        // input layout des sommets
        pImmediateContext->IASetInputLayout( pVertexLayout );
        // Le sampler state
        ID3DX11EffectSamplerVariable* variableSampler;
        variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();

        variableSampler->SetSampler(0, pSampleState);
        ID3DX11EffectShaderResourceVariable* variableTexture;
        variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
        // Activation de la texture
        variableTexture->SetResource(pResourceView);
        pPasse->Apply(0, pImmediateContext);
        // **** Rendu de l’objet
        pImmediateContext->Draw( 6, 0 );
    }

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

    
    void CPanneauPE::InitEffet()
    {
        // Compilation et chargement du vertex shader
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
        // Pour l’effet
        ID3DBlob* pFXBlob = nullptr;
        DXEssayer( D3DCompileFromFile( L"PostEffect.fx", 0, 0, 0, 
        "fx_5_0", 0, 0, 
        &pFXBlob, 0),
        DXE_ERREURCREATION_FX);
        D3DX11CreateEffectFromMemory( 
         pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, 
        pD3DDevice, &pEffet);
        pFXBlob->Release();
        pTechnique = pEffet->GetTechniqueByIndex(0); 
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
    }
}

