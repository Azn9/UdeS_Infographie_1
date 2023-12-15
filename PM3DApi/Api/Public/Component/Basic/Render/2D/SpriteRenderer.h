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

        XMFLOAT2 GetPositionOffset() const { return positionOffset; }
        void SetPositionOffset(const XMFLOAT2& positionOffset) { this->positionOffset = positionOffset; }

    private:
        XMFLOAT2 positionOffset = XMFLOAT2(0.0f, 0.0f);
        PM3D::CTexture* texture;
        std::unique_ptr<Shader> shader;
        DirectX::XMMATRIX matWVP;
        float textureSizeX;
        float textureSizeY;
    };
}
