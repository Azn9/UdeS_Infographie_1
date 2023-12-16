#include "Api/Public/Component/UIComponent.h"

#include <iostream>
#include <ostream>

#include "Core/Public/Core/MoteurWindows.h"
#include "Core/Public/Util/Time.h"

PM3D_API::UIComponent::~UIComponent()
{
    if (parentObject)
        std::cout << "UIComponent::~UIComponent() on " << parentObject->GetName() << std::endl;
}

void PM3D_API::UIComponent::Initialize()
{
    // Do nothing by default
}

void PM3D_API::UIComponent::Update()
{
    // Do nothing by default
}

void PM3D_API::UIComponent::UpdateMatrix()
{
    // Do nothing by default
}

void PM3D_API::UIComponent::DrawSelf() const
{
    // Do nothing by default
}

void PM3D_API::UIComponent::LogBeginDrawSelf() const
{
    beginDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

void PM3D_API::UIComponent::LogEndDrawSelf() const
{
    endDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}
