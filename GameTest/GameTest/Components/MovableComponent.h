#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Api/Public/EventSystem/EventSystem.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Objects/LoadingScene/CustomCube.h"
#include "Core/Imgui/imgui.h"
#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Input/Input.h"
#include "GameTest/Event/GameOverEvent.h"
#include <Api/Public/EventSystem/InTunnelEvent.h>
#include <Api/Public/EventSystem/CheckpointEvents.h>
#include <Api/Public/EventSystem/RespawnSkierEvent.h>

class MovableComponent final : public PM3D_API::Component
{
public:
	MovableComponent()
	{
		PM3D_API::EventSystem::Subscribe([this](const RestartEvent&)
		{
			physx::PxRigidDynamic* rigidDyn = parentObject->GetComponent<PM3D_API::Rigidbody>()->getRigidDynamic();
			rigidDyn->setLinearVelocity({0.f,0.f,0.f});
			rigidDyn->setAngularVelocity({0.f,0.f,0.f});
			parentObject->SetWorldPosition(XMFLOAT3(0.f,0.f,0.f));
			_inTunnel = false;
			//PM3D_API::EventSystem::Publish(RespawnSkierEvent());
		});
		PM3D_API::EventSystem::Subscribe([this](const InTunnelEvent&)
			{
				_inTunnel = true;
			});
	}

	void Update() override
	{
		if (parentObject->GetWorldPosition().z < -0.f)
			PM3D_API::EventSystem::Publish(CheckpointEvent_1(true));
		if (parentObject->GetWorldPosition().z < -250.f)
			PM3D_API::EventSystem::Publish(CheckpointEvent_2(true));
		if (parentObject->GetWorldPosition().z < -800.f)
			PM3D_API::EventSystem::Publish(CheckpointEvent_3(true));
		if (parentObject->GetWorldPosition().z < -1200.f)
			PM3D_API::EventSystem::Publish(CheckpointEvent_4(true));
		if (parentObject->GetWorldPosition().z < -1700.f)
			PM3D_API::EventSystem::Publish(CheckpointEvent_5(true));
		/*if (parentObject->GetWorldPosition().z < -1730.f)   //temporaire pour les tests
			PM3D_API::EventSystem::Publish(GameOverEvent(true));*/
		physx::PxRigidDynamic* rigidDyn = parentObject->GetComponent<PM3D_API::Rigidbody>()->getRigidDynamic();
		float vel = rigidDyn->getLinearVelocity().magnitude();

		PM3D_API::GameHost::GetInstance()->GetPostEffectPlane()->SetShaderVariableValue("velocity", vel);
	}

	void PhysicsUpdate() override
	{
		PM3D_API::Rigidbody* rigidBody = parentObject->GetComponent<PM3D_API::Rigidbody>();
		physx::PxRigidDynamic* rigidDynamic = rigidBody->getRigidDynamic();
		
		if (Input::IsKeyHeld(KeyCode::ARROW_LEFT)) // Ou IsKeyHeld
		{
			rigidDynamic->addForce(physx::PxVec3(static_cast<float>(-_speed), 0.f, 0.f));
		}
		if (Input::IsKeyHeld(KeyCode::ARROW_RIGHT)) // Ou IsKeyHeld
		{
			rigidDynamic->addForce(physx::PxVec3(static_cast<float>(_speed), 0.f, 0.f));
		}
		if (Input::IsKeyHeld(KeyCode::ARROW_UP) && !_inTunnel)
		{
			rigidDynamic->addForce(physx::PxVec3(0.f, 0.f, -static_cast<float>(_speed*2)));
		}
	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Speed");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_speed).c_str());
	}

private:
	int _speed = 70;
	bool _inTunnel = false;
};
