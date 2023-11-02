#include "../../Public/Component/Component.h"

#include <iostream>
#include <ostream>

#include "../../../../PetitMoteur3D/Core/Public/Core/MoteurWindows.h"
#include "../../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "Api/Public/GameObject/GameObject.h"

PM3D_API::Component::~Component()
{
	if (parentObject)
		std::cout << "Component::~Component() on " << parentObject->GetName() << std::endl;
}

void PM3D_API::Component::Initialize()
{
	// Do nothing by default
}

void PM3D_API::Component::Update()
{
	// Do nothing by default
}

void PM3D_API::Component::PhysicsUpdate()
{
	// Do nothing by default
}

void PM3D_API::Component::DrawSelf() const
{
	// Do nothing by default
}

void PM3D_API::Component::LogBeginDrawSelf() const
{
	beginDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

void PM3D_API::Component::LogEndDrawSelf() const
{
	endDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

