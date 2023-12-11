#pragma once
#include <cvt/wstring>
#include <string>
#include <set>

#include "Core/Public/Util/util.h"

#include "objet3d.h"

#include "Core/Public/Shader/d3dx11effect.h"

namespace PM3D
{
    template<class T>
    concept is_shader_param = std::_Is_any_of_v
        <T, float, XMFLOAT3, XMFLOAT4, XMVECTOR, ID3D11SamplerState*, ID3D11ShaderResourceView*>;
    
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
        
        template<is_shader_param T>
        void SetShaderVariableValue(const std::string& name, const T& param)
        {
            DXEssayer(SetShaderVar(name, param));
        }
        
        void enableAllPostEffects();
        std::set<int>& getEnabledPostEffects();

        const D3DX11_EFFECT_DESC* getEffectDesc() const;

        void Resize(const unsigned& width, const unsigned& height);
        void CreateDescriptions();
        void CreateTexturesAndViews();

    private:
        
        void InitEffet();
        static CSommetPanneauPE sommets[3];
        ID3D11Buffer* pVertexBuffer;
        CDispositifD3D11* pDispositif;
        
        // Pour les effets
        ID3DX11Effect* pEffet;
        ID3DX11EffectTechnique* pTechnique;
        ID3DX11EffectPass* pPasse;
        ID3D11SamplerState* pSampleState;

        //Descriptions des texture et vues
        D3D11_TEXTURE2D_DESC pTextureDesc;
        D3D11_RENDER_TARGET_VIEW_DESC pRenderTargetViewDesc;
        D3D11_SHADER_RESOURCE_VIEW_DESC pShaderResourceViewDesc;
        
        // Textures de rendu pour effets
        ID3D11Texture2D* pTmpTexture;
        ID3D11Texture2D* pTmp2Texture;
        ID3D11Texture2D* pTmpDepthTexture;
        ID3D11RenderTargetView* pTmpRenderTargetView;
        ID3D11ShaderResourceView* pTmpResourceView;

        ID3D11RenderTargetView* pTmp2RenderTargetView;
        ID3D11ShaderResourceView* pTmp2ResourceView;
        
        ID3D11RenderTargetView* pMainRenderTargetView;
        
        ID3D11RenderTargetView* pCurrentRenderTargetView;
        ID3D11ShaderResourceView* pCurrentResourceView;

        D3DX11_EFFECT_DESC* pEffectDesc;
        std::set<int> pEnabledTechniques;
        
        ID3D11InputLayout* pVertexLayout;

        //Fonctions pour enregistrer les variables de shader
        HRESULT SetShaderVar(const std::string& name, const float& f) const;
        HRESULT SetShaderVar(const std::string& name, const XMFLOAT4& fs) const;
        HRESULT SetShaderVar(const std::string& name, const XMFLOAT3& fs) const;
        HRESULT SetShaderVar(const std::string& name, const XMVECTOR& fs) const;
        HRESULT SetShaderVar(const std::string& name, ID3D11SamplerState* s) const;
        HRESULT SetShaderVar(const std::string& name, ID3D11ShaderResourceView* s) const;
        
    };
} // namespace PM3D