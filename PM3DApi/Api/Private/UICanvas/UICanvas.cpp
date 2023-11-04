#include "../../Public/UICanvas/UICanvas.h"

#include <iostream>

#include "../../../../PetitMoteur3D/Core/Public/Core/MoteurWindows.h"
#include "../../Public/EventSystem/EventSystem.h"
#include "../../Public/EventSystem/Basic/WindowResizeEvent.h"

PM3D_API::UICanvas::UICanvas() : UIObject(
    "Canvas",
    nullptr,
    {
        static_cast<float>(GameHost::GetInstance()->GetDispositif()->largeurEcran),
        static_cast<float>(GameHost::GetInstance()->GetDispositif()->hauteurEcran)
    },
    {0.f, 0.f},
    ScaleMode::NONE_RELATIVE,
    Anchor::TOP_LEFT,
    Anchor::TOP_LEFT
)
{
    EventSystem::Subscribe([&](const WindowResizeEvent& event)
    {
        SetScreenScale(DirectX::XMFLOAT2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight())));
    });
}

PM3D_API::UICanvas::~UICanvas()
{
    isDeleted = true;

    std::cout << "UICanvas::~UICanvas()" << std::endl;
    children.clear();
}

void PM3D_API::UICanvas::Initialize()
{
    // Do nothing by default
}

void PM3D_API::UICanvas::Update()
{
    if (isDeleted) return;
    UIObject::Update();
}

void PM3D_API::UICanvas::Draw()
{
    if (isDeleted) return;
    UIObject::Draw();
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
    child->parent = this;
    child->canvas = this;
    children.push_back(std::move(child));
}
