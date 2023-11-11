#include "Api/Public/GameObject/UICanvas.h"

#include <iostream>

#include "Core/Public/Core/MoteurWindows.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/Basic/WindowResizeEvent.h"

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
    // Do nothing by default
}

void PM3D_API::UICanvas::Update()
{
    if (isDeleted) return;

    for (const auto & uiObject : children)
    {
        if (uiObject)
            uiObject->Update();
    }
}

void PM3D_API::UICanvas::Draw()
{
    if (isDeleted) return;
    
    for (const auto & uiObject : children)
    {
        if (uiObject)
            uiObject->Draw();
    }
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

    for (const auto & uiObject : children)
    {
        if (uiObject)
            uiObject->UpdateScaleAndPosition();
    }
}
