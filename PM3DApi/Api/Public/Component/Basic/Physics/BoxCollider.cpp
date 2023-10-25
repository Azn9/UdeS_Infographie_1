#include "BoxCollider.h"

#include "Rigidbody.h"
#include "Api/Public/GameHost.h"

void PM3D_API::BoxCollider::Initialize()
{
	Collider::Initialize();

	const auto pxPhysics = GameHost::GetInstance()->GetScene()->GetPhysicsResolver()->GetPhysics();

	const auto worldScale = parentObject->GetWorldScale();
	shape = pxPhysics->createShape(physx::PxBoxGeometry{ worldScale.x, worldScale.y, worldScale.z }, *material, true);

	// Set position and rotation
	const auto worldPosition = parentObject->GetWorldPosition();
	const auto worldRotation = parentObject->GetWorldRotationQuaternion();

	const auto rigidbody = parentObject->GetComponent<Rigidbody>(); // Ne peut pas être null, vérifié dans Collider::Initialize()

	const auto actor = rigidbody->GetActor();
	const auto pxRigidbody = static_cast<physx::PxRigidBody*>(actor);

	const auto globalPos = physx::PxTransform(
		physx::PxVec3(worldPosition.x, worldPosition.y, worldPosition.z),
		physx::PxQuat(worldRotation.x, worldRotation.y, worldRotation.z, worldRotation.w)
	);

	pxRigidbody->attachShape(*shape);
	pxRigidbody->setGlobalPose(globalPos);
}
