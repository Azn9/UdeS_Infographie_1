#pragma once
#include <DirectXMath.h>

#include "Api/Public/Component/UIComponent.h"
#include "Api/Public/Util/Font/FontLoader.h"

namespace PM3D_API
{
class TextRenderer : public UIComponent
{
public:
	TextRenderer(const FontLoader& fontLoader, std::string text, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT3& color, const float scale = 1.0f)
		: text(std::move(text)), fontLoader(fontLoader), position(position), color(color) {}

	void SetText(const std::string& text) { this->text = text; }

	void DrawSelf() const override;

private:
	std::string text;
	const FontLoader& fontLoader;
	const DirectX::XMFLOAT2 position;
	const DirectX::XMFLOAT3 color;
};
}
