#pragma once
#include <map>
#include <cvt/wstring>
#include <string>
#include <variant>

#include "objet3d.h"

#include "Core/Public/Shader/d3dx11effect.h"

namespace PM3D
{
    template<class T>
    concept is_shader_param = std::_Is_any_of_v
        <T, float, XMFLOAT4, XMVECTOR, ID3D11SamplerState*, ID3D11ShaderResourceView*>;
    
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
        using ShaderParam = std::variant
            <float, XMFLOAT4, XMVECTOR, ID3D11SamplerState*, ID3D11ShaderResourceView*>;
        explicit CPanneauPE(CDispositifD3D11* pDispositif_in);
        virtual ~CPanneauPE();
        virtual void Draw() override;
        void DebutPostEffect();
        void FinPostEffect();
        
        template<is_shader_param T>
        void AddShaderVariableValue(const std::string& name, const T& param);
        
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
        ID3D11Texture2D* pTmp2Texture;
        ID3D11RenderTargetView* pTmpRenderTargetView;
        ID3D11ShaderResourceView* pTmpResourceView;

        ID3D11RenderTargetView* pTmp2RenderTargetView;
        ID3D11ShaderResourceView* pTmp2ResourceView;
        
        ID3D11RenderTargetView* pMainRenderTargetView;
        
        ID3D11RenderTargetView* pCurrentRenderTargetView;
        ID3D11ShaderResourceView* pCurrentResourceView;
        
        static const int NOMBRE_TECHNIQUES = 3;
        ID3D11InputLayout* pVertexLayout;

        std::map<std::string, ShaderParam> pPersistentShaderVars;

        struct SetVariableVisitor
        {
            const std::string name;
            ID3DX11Effect* effect;
            //SetVariableVisitor() = delete;
            SetVariableVisitor(const std::string& name, ID3DX11Effect* effect) : name(name), effect(effect) {}
            
            HRESULT operator()(const float& f) const;
            HRESULT operator()(const XMFLOAT4& fs) const;
            HRESULT operator()(const XMVECTOR& fs) const;
            HRESULT operator()(ID3D11SamplerState* s) const;
            HRESULT operator()(ID3D11ShaderResourceView* s) const;
        };
    };
} // namespace PM3D