#pragma once
#include <DirectXMath.h>

#include "Api/Public/Component/UIComponent.h"
#include "Api/Public/Shader/Basic/TextShader.h"
#include "Api/Public/Util/Font/FontLoader.h"

namespace PM3D_API
{
    class TextRenderer final : public UIComponent
    {
    public:
        TextRenderer(
            const FontLoader& fontLoader,
            std::string text,
            const DirectX::XMFLOAT2& positionOffset,
            const DirectX::XMFLOAT3& color,
            const float scale = 1.0f
        );

        void SetText(const std::string& text) { this->text = text; }
        void SetScale(const float scale) { this->scale = scale; }
        DirectX::XMFLOAT2 GetPositionOffset() const { return positionOffset; }
        void SetPositionOffset(const DirectX::XMFLOAT2& positionOffset) { this->positionOffset = positionOffset; }

        void Initialize() override;
        void RefreshData();
        void DrawSelf() const override;

    private:
        std::string text;
        FontLoader fontLoader;
        DirectX::XMFLOAT2 positionOffset;
        DirectX::XMFLOAT3 color;
        float scale;
        std::unique_ptr<TextShader> shader;
        std::vector<TextShader::TextShaderParameters> parameters;
    };
}
