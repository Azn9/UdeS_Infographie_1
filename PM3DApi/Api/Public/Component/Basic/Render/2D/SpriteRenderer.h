#pragma once
#include <string>

#include "../../../UIComponent.h"
#include "../../../../Shader/Shader.h"

namespace PM3D
{
    class CTexture;
}

namespace PM3D_API
{
    class SpriteRenderer final : public UIComponent
    {
    public:
        explicit SpriteRenderer(const std::wstring& textureName);

        void Initialize() override;
        void Update() override;
        void DrawSelf() const override;
        void UpdateMatrix() override;

    private:
        PM3D::CTexture* texture;
        std::unique_ptr<Shader> shader;
        DirectX::XMMATRIX matWVP;
        float textureSizeX;
        float textureSizeY;
    };
}
