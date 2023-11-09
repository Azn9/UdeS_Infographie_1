#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Imgui/imgui.h"
#include "Component/Component.h"
#include "GameObject/GameObject.h"
#include "Component/Basic/Physics/SphereCollider.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/CollisionObstacleEvent.h"

class SizeModifierComponent final : public PM3D_API::Component
{
public:

	SizeModifierComponent() {
		PM3D_API::EventSystem::Subscribe([this](const CollisionObstacleEvent& event)
			{
				_collisionHappend = true;
			});
	}

	void Update() override
	{
		DirectX::XMFLOAT3 preScale = parentObject->GetWorldScale();
		physx::PxShape* shape = parentObject->GetComponent<PM3D_API::SphereCollider>()->getShape();

		if (_collisionHappend)
		{
			parentObject->SetWorldScale(DirectX::XMFLOAT3(
				preScale.x * 0.75f,
				preScale.y * 0.75f,
				preScale.z * 0.75f
			));
			_collisionHappend = false;
			shape->setGeometry(physx::PxSphereGeometry(preScale.x * .5f));
		}
		else
		{
			parentObject->SetWorldScale(DirectX::XMFLOAT3(
				preScale.x * _sizeModificationSpeed,
				preScale.y * _sizeModificationSpeed,
				preScale.z * _sizeModificationSpeed
			));
			shape->setGeometry(physx::PxSphereGeometry(preScale.x * _sizeModificationSpeed));
		}

	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Speed");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_sizeModificationSpeed).c_str());
	}

private:
	float _sizeModificationSpeed = 1.001f;
	bool _collisionHappend = false;
};