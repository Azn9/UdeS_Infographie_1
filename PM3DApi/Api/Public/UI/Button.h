#pragma once
#include <functional>
#include <string>
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/GameObject/UIObject.h"

class Button final : public PM3D_API::UIObject
{
public:
    Button(
        std::wstring baseFileName,
        std::wstring hoverFileName,
        std::wstring pressedFileName,
        const float x,
        const float y,
        const float width,
        const float height,
        std::string text,
        const std::function<void()>& callback
    ) : UIObject("Button", {width, height}, {x, y}, 0, true, true),
        baseFileName(std::move(baseFileName)),
        hoverFileName(std::move(hoverFileName)),
        pressedFileName(std::move(pressedFileName)),
        text(std::move(text)),
        spriteRendererPtr(nullptr),
        hoverSpriteRendererPtr(nullptr),
        pressedSpriteRendererPtr(nullptr),
        callback(callback)
    {
    }

    void Initialize() override;

    void OnHoverEnter() override;
    void OnClickPressed() override;

    void Draw() override;

private:
    std::wstring baseFileName;
    std::wstring hoverFileName;
    std::wstring pressedFileName;
    std::string text;
    std::unique_ptr<PM3D_API::SpriteRenderer>::pointer spriteRendererPtr;
    std::unique_ptr<PM3D_API::SpriteRenderer>::pointer hoverSpriteRendererPtr;
    std::unique_ptr<PM3D_API::SpriteRenderer>::pointer pressedSpriteRendererPtr;
    std::function<void()> callback;
};
