#include "../../Public/UICanvas/UICanvas.h"

#include <iostream>

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
