#include "Button.h"

#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/Util/Sound/SoundManager.h"

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

    // TODO : create text
}

void Button::OnHoverEnter()
{
    SoundManager::GetInstance().Play(SoundManager::GetInstance().uiRollover1Buffer);
}

void Button::OnClickPressed()
{
    SoundManager::GetInstance().Play(SoundManager::GetInstance().uiClick1Buffer);
    callback();
}

void Button::Draw()
{
    if (text != "")
    {
        // TODO : draw text
    }

    if (isClicked)
    {
        pressedSpriteRendererPtr->DrawSelf();
        return;
    }

    if (isHovered)
    {
        hoverSpriteRendererPtr->DrawSelf();
        return;
    }

    spriteRendererPtr->DrawSelf();
}
