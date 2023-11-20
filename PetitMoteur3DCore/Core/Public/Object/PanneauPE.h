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
        ID3D11InputLayout* pVertexLayout;
        ID3D11SamplerState* pSampleState;

        ID3D11ShaderResourceView* pResourceView;
    };
} // namespace PM3D