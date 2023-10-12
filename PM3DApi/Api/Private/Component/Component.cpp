#include "Api/Public/Component/Component.h"

void PM3D_API::Component::Initialize()
{
	// Do nothing by default
}

void PM3D_API::Component::Update(double deltaTime)
{
	// Do nothing by default
}

void PM3D_API::Component::FixedUpdate(double fixedDeltaTime)
{
	// Do nothing by default
}

void PM3D_API::Component::DrawSelf() const
{
	// Do nothing by default
}

void PM3D_API::Component::SetGameObject(const std::shared_ptr<GameObject> gameObject)
{
	parentObject = gameObject;
}
