#include "Button.h"

#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/Component/Basic/Render/2D/TextRenderer.h"
#include "Api/Public/Util/Sound/SoundManager.h"
#include "Core/Public/Core/MoteurWindows.h"

void Button::Initialize()
{
    auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(baseFileName);
    spriteRendererPtr = spriteRenderer.get();
    AddComponent(std::move(spriteRenderer));
    spriteRendererPtr->Initialize();

    auto hoverSpriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(hoverFileName);
    hoverSpriteRendererPtr = hoverSpriteRenderer.get();
    AddComponent(std::move(hoverSpriteRenderer));
    hoverSpriteRendererPtr->Initialize();

    auto pressedSpriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(pressedFileName);
    pressedSpriteRendererPtr = pressedSpriteRenderer.get();
    AddComponent(std::move(pressedSpriteRenderer));
    pressedSpriteRendererPtr->Initialize();

    if (text == "") return;

    auto textRenderer = std::make_unique<PM3D_API::TextRenderer>(
        fontLoader,
        text,
        positionOffset,
        DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
        scale
    );
    textRendererPtr = textRenderer.get();
    AddComponent(std::move(textRenderer));
    textRendererPtr->Initialize();
}

void Button::OnHoverEnter()
{
    if (alpha == 0) return;

    SoundManager::GetInstance().Play(SoundManager::GetInstance().uiRollover1Buffer);
    PM3D::CMoteurWindows::GetInstance().SetCursorClick();
}

void Button::OnHoverExit()
{
    if (alpha == 0) return;

    PM3D::CMoteurWindows::GetInstance().SetCursorDefault();
}

void Button::OnClickPressed()
{
    if (alpha == 0) return;

    if (textRendererPtr)
    {
        textRendererPtr->SetPositionOffset(DirectX::XMFLOAT2(
            textRendererPtr->GetPositionOffset().x,
            textRendererPtr->GetPositionOffset().y - 5
        ));
        textRendererPtr->RefreshData();
    }
    SoundManager::GetInstance().Play(SoundManager::GetInstance().uiClick1Buffer);

    callback();
}

void Button::OnClickReleased()
{
    if (alpha == 0) return;

    if (textRendererPtr)
    {
        textRendererPtr->SetPositionOffset(DirectX::XMFLOAT2(
            textRendererPtr->GetPositionOffset().x,
            textRendererPtr->GetPositionOffset().y + 5
        ));
        textRendererPtr->RefreshData();
    }
}

void Button::Draw()
{
    if (alpha == 0) return;

    if (isClicked)
    {
        pressedSpriteRendererPtr->DrawSelf();
    }
    else if (isHovered)
    {
        hoverSpriteRendererPtr->DrawSelf();
    }
    else
    {
        spriteRendererPtr->DrawSelf();
    }

    if (text != "" && textRendererPtr)
    {
        textRendererPtr->DrawSelf();
    }
}

void Button::SetAlpha(const float newAlpha)
{
    UIObject::SetAlpha(newAlpha);

    if (textRendererPtr)
    {
        textRendererPtr->RefreshData();
    }
}
