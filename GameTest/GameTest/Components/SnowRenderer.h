#pragma once
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "GameTest/Shader/SnowShader.h"

namespace PM3D
{
    class CTexture;
}

class SnowRenderer final : public PM3D_API::MeshRenderer
{
public:
    SnowRenderer(std::unique_ptr<SnowShader>&& shader, const std::string& meshName, const bool tesselate = false) :
        MeshRenderer(std::move(shader), meshName), snowRVT(nullptr), stagingTexture(nullptr),
        snowRVTResourceView(nullptr),
        snowRVTRTV(nullptr),
        sparklesTexture(nullptr),
        tesselate(tesselate)
    {
    }

    void Initialize() override;
    void DrawSelf() const override;

private:
    void DrawRVT() const;
    ID3D11Texture2D* snowRVT;
    ID3D11Texture2D* stagingTexture;
    ID3D11ShaderResourceView* snowRVTResourceView;
    ID3D11RenderTargetView* snowRVTRTV;
    PM3D::CTexture* sparklesTexture;
    bool tesselate = false;
};
