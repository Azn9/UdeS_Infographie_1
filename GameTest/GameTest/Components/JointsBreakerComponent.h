#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Core/Imgui/imgui.h"
#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Component/Basic/Physics/SkierCollider.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/CollisionSkierEvent.h"
#include "GameTest/Event/GameOverEvent.h"
#include <Api/Public/EventSystem/InTunnelEvent.h>
#include <Api/Public/Component/Basic/Physics/Rigidbody.h>

class JointsBreakerComponent final : public PM3D_API::Component
{
public:

	JointsBreakerComponent() {
		PM3D_API::EventSystem::Subscribe([this](const CollisionSkierEvent& event)
			{
				_collisionHappend = true;
				_id = event.GetId();
			});
	}

	void PhysicsUpdate() override
	{

		if (_collisionHappend)
		{
			physx::PxShape* skieurShape = parentObject->GetComponent<PM3D_API::SkierCollider>()->getShape();
			if (skieurShape->getSimulationFilterData().word1 == _id)
			{
				physx::PxConstraint* joint1;
				auto nbWritten1 = parentObject->GetComponent<PM3D_API::Rigidbody>()->GetActor()->getConstraints(&joint1, 1, 0);
				if (nbWritten1 != 0) {
					physx::PxRigidActor* skier;
					physx::PxRigidActor* ski;
					joint1->getActors(skier, ski);
					static_cast<physx::PxRigidDynamic*>(skier)->setLinearVelocity(physx::PxVec3(0.f, 15.f, 5.f));
					static_cast<physx::PxRigidDynamic*>(skier)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
					static_cast<physx::PxRigidDynamic*>(skier)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
					static_cast<physx::PxRigidDynamic*>(skier)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);
					static_cast<physx::PxRigidDynamic*>(skier)->setAngularVelocity(physx::PxVec3(1.f, 2.f, 3.f));

					parentObject->GetComponent<PM3D_API::Rigidbody>()->getRigidDynamic()->setLinearVelocity(physx::PxVec3(0.f, 15.f, -5.f));

					joint1->release();
				}
			/*	else {
					parentObject->GetComponent<PM3D_API::Rigidbody>()->GetActor()->detachShape(*skieurShape);
				}*/

				_collisionHappend = false;
			}
		}

	}

private:

	bool _collisionHappend = false;
	int _id{};

};
