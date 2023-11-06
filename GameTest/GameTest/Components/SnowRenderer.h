#pragma once
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/MeshRenderer.h"

class SnowRenderer : public PM3D_API::MeshRenderer
{
public:
    SnowRenderer(std::unique_ptr<PM3D_API::Shader>&& shader, const std::string& meshName) : MeshRenderer(std::move(shader), meshName) {}

    void Initialize() override;
    void DrawSelf() const override;

private:
    void DrawRVT() const;
};
