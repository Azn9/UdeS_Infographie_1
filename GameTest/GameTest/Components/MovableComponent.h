#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Api/Public/EventSystem/EventSystem.h"
#include "Imgui/imgui.h"
#include "Component/Component.h"
#include "GameObject/GameObject.h"
#include "Component/Basic/Physics/Rigidbody.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Objects/LoadingScene/CustomCube.h"
#include "Input/Input.h"

class MovableComponent final : public PM3D_API::Component
{
public:
	void MovableComponent()
	{
		PM3D_API::EventSystem::Subscribe([&parentObject](const RestartEvent&)
		{
			parentObject->SetWorldPosition(XMFLOAT3(0.f,0.f,0.f));
		});
	}
	
	void Update() override
	{
		PM3D_API::Rigidbody* rigidBody = parentObject->GetComponent<PM3D_API::Rigidbody>();
		physx::PxRigidDynamic* rigidDynamic = rigidBody->getRigidDynamic();
		
		if (Input::IsKeyHeld(KeyCode::ARROW_LEFT)) // Ou IsKeyHeld
		{
			rigidDynamic->addForce(physx::PxVec3(_speed, 0, 0));
		}
		if (Input::IsKeyHeld(KeyCode::ARROW_RIGHT)) // Ou IsKeyHeld
		{
			rigidDynamic->addForce(physx::PxVec3(-_speed, 0, 0));
		}

	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Speed");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_speed).c_str());
	}

private:
	int _speed = 500;
};
