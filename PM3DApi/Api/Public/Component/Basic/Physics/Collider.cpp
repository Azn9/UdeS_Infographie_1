#include "Collider.h"

#include <stdexcept>

#include "Rigidbody.h"
#include "Api/Public/GameHost.h"
#include "Api/Public/GameObject/GameObject.h"

void PM3D_API::Collider::Initialize()
{
	if (!GameHost::GetInstance()->GetScene()->GetPhysicsResolver())
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
