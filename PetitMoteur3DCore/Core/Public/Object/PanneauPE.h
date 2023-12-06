#pragma once
#include "objet3d.h"

#include "Core/Public/Shader/d3dx11effect.h"

namespace PM3D
{
    class CDispositifD3D11;
    class CSommetPanneauPE
    {
    public:
        CSommetPanneauPE() = default;
        CSommetPanneauPE(const XMFLOAT3& position, const XMFLOAT2& coordTex)
        : m_position(position)
        , m_coordTex(coordTex)
        { }
    public:
        static UINT numElements;
        static D3D11_INPUT_ELEMENT_DESC layout[];
        XMFLOAT3 m_position;
        XMFLOAT2 m_coordTex;
    };

    
    // Classe : CPanneauPE
    // BUT : Classe pour les post-effects
    class CPanneauPE : public CObjet3D
    {
    public:
        explicit CPanneauPE(CDispositifD3D11* pDispositif_in);
        virtual ~CPanneauPE();
        virtual void Draw() override;
        void DebutPostEffect();
        void FinPostEffect();
    private:
        void InitEffet();
        static CSommetPanneauPE sommets[6];
        ID3D11Buffer* pVertexBuffer;
        CDispositifD3D11* pDispositif;
        
        // Pour les effets
        ID3DX11Effect* pEffet;
        ID3DX11EffectTechnique* pTechnique;
        ID3DX11EffectPass* pPasse;
        ID3D11SamplerState* pSampleState;

        // Texture de rendu pour effets
        ID3D11Texture2D* pTmpTexture;
        ID3D11RenderTargetView* pTmpRenderTargetView;
        ID3D11ShaderResourceView* pTmpResourceView;
        
        ID3D11RenderTargetView* pMainRenderTargetView;
        ID3D11ShaderResourceView* pMainResourceView;
        
        ID3D11RenderTargetView* pCurrentTargetView;
        ID3D11ShaderResourceView* pCurrentResourceView;
        
        static const int NOMBRE_TECHNIQUES = 2;
        ID3D11InputLayout* pVertexLayout;
    };
} // namespace PM3D