#pragma once
#include <functional>
#include <string>

#include "Button.h"
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/GameObject/UIObject.h"
#include "Api/Public/Util/Font/FontLoader.h"

class DisabledButton final : public Button
{
public:
    DisabledButton(
        const std::wstring& baseFileName,
        const float x,
        const float y,
        const float width,
        const float height,
        PM3D_API::FontLoader fontLoader,
        std::string text,
        const DirectX::XMFLOAT2 positionOffset,
        const float scale
    ) : Button(baseFileName, baseFileName, baseFileName, x, y, width, height, std::move(fontLoader), std::move(text),
               positionOffset, scale, [
               ]()
               {
               })
    {
    }

    void OnHoverEnter() override
    {
    }

    void OnHoverExit() override
    {
    }

    void OnClickPressed() override
    {
    }

    void OnClickReleased() override
    {
    }
};
