#pragma once
#include <functional>
#include <string>
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/Component/Basic/Render/2D/TextRenderer.h"
#include "Api/Public/GameObject/UIObject.h"
#include "Api/Public/Util/Font/FontLoader.h"

class Button : public PM3D_API::UIObject
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
        PM3D_API::FontLoader fontLoader,
        std::string text,
        const DirectX::XMFLOAT2 positionOffset,
        const float scale,
        const std::function<void()>& callback
    ) : UIObject("Button", {width, height}, {x, y}, 0, true, true),
        baseFileName(std::move(baseFileName)),
        hoverFileName(std::move(hoverFileName)),
        pressedFileName(std::move(pressedFileName)),
        text(std::move(text)),
        positionOffset(positionOffset),
        fontLoader(std::move(fontLoader)),
        scale(scale),
        callback(callback),
        spriteRendererPtr(nullptr),
        hoverSpriteRendererPtr(nullptr),
        pressedSpriteRendererPtr(nullptr),
        textRendererPtr(nullptr)
    {
    }

    Button(
        std::wstring baseFileName,
        std::wstring hoverFileName,
        std::wstring pressedFileName,
        const float x,
        const float y,
        const float width,
        const float height,
        const std::function<void()>& callback
    ) : UIObject("Button", {width, height}, {x, y}, 0, true, true),
        baseFileName(std::move(baseFileName)),
        hoverFileName(std::move(hoverFileName)),
        pressedFileName(std::move(pressedFileName)),
        callback(callback),
        spriteRendererPtr(nullptr),
        hoverSpriteRendererPtr(nullptr),
        pressedSpriteRendererPtr(nullptr),
        textRendererPtr(nullptr)
    {
    }

    void Initialize() override;

    void OnHoverEnter() override;
    void OnHoverExit() override;
    void OnClickPressed() override;
    void OnClickReleased() override;

    void Draw() override;

    void SetAlpha(const float newAlpha) override;

private:
    std::wstring baseFileName;
    std::wstring hoverFileName;
    std::wstring pressedFileName;

    std::string text;
    DirectX::XMFLOAT2 positionOffset = DirectX::XMFLOAT2(0.0f, 0.0f);
    PM3D_API::FontLoader fontLoader;
    float scale = 0.f;

    std::function<void()> callback;

    std::unique_ptr<PM3D_API::SpriteRenderer>::pointer spriteRendererPtr;
    std::unique_ptr<PM3D_API::SpriteRenderer>::pointer hoverSpriteRendererPtr;
    std::unique_ptr<PM3D_API::SpriteRenderer>::pointer pressedSpriteRendererPtr;
    std::unique_ptr<PM3D_API::TextRenderer>::pointer textRendererPtr;
};
