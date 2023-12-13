#include "Api/Public/GameObject/UICanvas.h"

#include <iostream>

#include "Core/Public/Core/MoteurWindows.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/Basic/WindowResizeEvent.h"

struct MouseClickEvent;

PM3D_API::UICanvas::UICanvas() : UIObject(
    "Canvas",
    {
        static_cast<float>(GameHost::GetInstance()->GetDispositif()->largeurEcran),
        static_cast<float>(GameHost::GetInstance()->GetDispositif()->hauteurEcran)
    },
    {0.f, 0.f}, 0.f, false, false
)
{
    EventSystem::Subscribe([&](const WindowResizeEvent& event)
    {
        SetScale(DirectX::XMFLOAT2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight())));
    });
}


PM3D_API::UICanvas::~UICanvas()
{
    isDeleted = true;

    std::cout << "UICanvas::~UICanvas()" << std::endl;
}

void PM3D_API::UICanvas::Initialize()
{
    EventSystem::Subscribe([&](const MouseClickEvent& event)
    {
        UpdateClick(event);
    });
}

void PM3D_API::UICanvas::Update()
{
    if (isDeleted) return;

    for (const auto& uiObject : children)
    {
        if (uiObject)
            uiObject->Update();
    }
}

void PM3D_API::UICanvas::Draw()
{
    if (isDeleted) return;

    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    pDispositif->DesactiverDepth();

    for (const auto& uiObject : children)
    {
        if (uiObject)
            uiObject->Draw();
    }

    pDispositif->ActiverDepth();
}

void PM3D_API::UICanvas::DrawSelf() const
{
    LogBeginDrawSelf();

    if (isDeleted) return;
    UIObject::DrawSelf();

    LogEndDrawSelf();
}

void PM3D_API::UICanvas::AddChild(std::unique_ptr<UIObject>&& child)
{
    std::cout << "UICanvas::AddChild(UIObject*) on " << name << std::endl;
    child->canvas = this;
    children.push_back(std::move(child));
}

void PM3D_API::UICanvas::SetScale(const DirectX::XMFLOAT2& newScale)
{
    UIObject::SetScale(newScale);

    for (const auto& uiObject : children)
    {
        if (uiObject)
            uiObject->UpdateScaleAndPosition();
    }
}

void PM3D_API::UICanvas::UpdateHover(const int x, const int y) const
{
    const int screenX = x;
    const int screenY = GameHost::GetInstance()->GetScreenHeight() - y;

    for (const auto& uiObject : children)
    {
        if (!uiObject) continue;

        {
            const int minX = static_cast<int>(uiObject->GetPosition().x);
            const int maxX = static_cast<int>(uiObject->GetPosition().x + uiObject->GetScale().x);
            const int minY = static_cast<int>(uiObject->GetPosition().y);
            const int maxY = static_cast<int>(uiObject->GetPosition().y + uiObject->GetScale().y);

            if (screenX >= minX && screenX <= maxX && screenY >= minY && screenY <= maxY)
            {
                if (!uiObject->isHovered)
                {
                    uiObject->isHovered = true;
                    uiObject->OnHoverEnter();
                }
            }
            else
            {
                if (uiObject->isHovered)
                {
                    uiObject->isHovered = false;
                    uiObject->OnHoverExit();
                }
            }
        }
    }
}

void PM3D_API::UICanvas::UpdateClick(const MouseClickEvent& event)
{
    const int screenX = event.x;
    const int screenY = GameHost::GetInstance()->GetScreenHeight() - event.y;
    const bool pressed = event.pressed;

    for (const auto& uiObject : children)
    {
        if (!uiObject) continue;

        {
            const int minX = static_cast<int>(uiObject->GetPosition().x);
            const int maxX = static_cast<int>(uiObject->GetPosition().x + uiObject->GetScale().x);
            const int minY = static_cast<int>(uiObject->GetPosition().y);
            const int maxY = static_cast<int>(uiObject->GetPosition().y + uiObject->GetScale().y);

            if (screenX >= minX && screenX <= maxX && screenY >= minY && screenY <= maxY)
            {
                if (pressed)
                {
                    if (!uiObject->isClicked)
                    {
                        uiObject->isClicked = true;
                        uiObject->OnClickPressed();
                    }
                }
                else if (uiObject->isClicked)
                {
                    uiObject->isClicked = false;
                    uiObject->OnClickReleased();
                }
            }
            else
            {
                if (uiObject->isClicked)
                {
                    uiObject->isClicked = false;
                    uiObject->OnClickReleased();
                }
            }
        }
    }
}
