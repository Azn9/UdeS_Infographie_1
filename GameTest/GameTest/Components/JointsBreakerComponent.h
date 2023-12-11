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
			});
	}

	void PhysicsUpdate() override
	{


		if (_collisionHappend)
		{
			physx::PxConstraint* joint1;
			physx::PxConstraint* joint2;
			auto nbWritten = parentObject->GetComponent<PM3D_API::Rigidbody>()->GetActor()->getConstraints(&joint1, 1, 0);
			//auto t2 = parentObject->GetComponent<PM3D_API::Rigidbody>()->GetActor()->getConstraints(&joint2, 1, 1);
			
			if (nbWritten != 0) joint1->release();
			//joint2->release();*/

			_collisionHappend = false;

		}

	}

private:

	bool _collisionHappend = false;

};
