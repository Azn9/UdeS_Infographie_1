#include "../../../../Public/Component/Basic/Physics/PlaneCollider.h"
#include "../../../../Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../../Public/GameHost.h"
#include <limits>

void PM3D_API::PlaneCollider::Initialize()
{
	Collider::Initialize();

	const auto pxPhysics = GameHost::GetInstance()->GetScene()->GetPhysicsResolver()->GetPhysics();

	const auto worldScale = parentObject->GetWorldScale();
	shape = pxPhysics->createShape(physx::PxBoxGeometry{ worldScale.x, std::numeric_limits<physx::PxReal>::epsilon(), worldScale.z }, *material, true);

	const auto rigidbody = parentObject->GetComponent<Rigidbody>(); // Ne peut pas être null, vérifié dans Collider::Initialize()

	const auto actor = rigidbody->GetActor();

	physx::PxShape* baseShape;
	actor->getShapes(&baseShape, 1, 0);
	
	actor->detachShape(*baseShape);
	actor->attachShape(*shape);

	// Set position and rotation
	const auto worldPosition = parentObject->GetWorldPosition();
	const auto worldRotation = parentObject->GetWorldRotationQuaternion();

	const auto globalPos = physx::PxTransform(
		physx::PxVec3(worldPosition.x, worldPosition.y, worldPosition.z),
		physx::PxQuat(worldRotation.x, worldRotation.y, worldRotation.z, worldRotation.w)
	);
	
	actor->setGlobalPose(globalPos);
}
