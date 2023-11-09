#include "Api/Public/Component/Basic/Physics/Collider.h"

#include <stdexcept>

#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/GameHost.h"
#include "Api/Public/GameObject/GameObject.h"

void PM3D_API::Collider::Initialize()
{
	if (!parentObject->GetScene()->GetPhysicsResolver())
	{
		throw std::runtime_error("Collider::Initialize() - No physics resolver found in scene");
	}

	const auto rigidBodies = parentObject->GetComponents<Rigidbody>();
	
	if (rigidBodies.empty())
	{
		throw std::runtime_error("Collider::Initialize() - No rigidbody found on parent object");
	}
	if (rigidBodies.size() > 1)
	{
		throw std::runtime_error("Collider::Initialize() - Multiple rigidbodies found on parent object");
	}
}
