#pragma once
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"

namespace PM3D
{
    class CTexture;
}

class SnowRenderer : public PM3D_API::MeshRenderer
{
public:
    SnowRenderer(std::unique_ptr<PM3D_API::Shader>&& shader, const std::string& meshName) : MeshRenderer(std::move(shader), meshName) {}

    void Initialize() override;
    void DrawSelf() const override;

private:
    void DrawRVT() const;
    ID3D11Texture2D* snowRVT;
    ID3D11Texture2D* stagingTexture;
    ID3D11ShaderResourceView* snowRVTResourceView;
    ID3D11RenderTargetView* snowRVTRTV;
    PM3D::CTexture* sparklesTexture;
};
