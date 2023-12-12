#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Core/Imgui/imgui.h"
#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Component/Basic/Physics/SphereCollider.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/CollisionObstacleEvent.h"
#include "GameTest/Event/GameOverEvent.h"
#include <Api/Public/EventSystem/InTunnelEvent.h>

class SizeModifierComponent final : public PM3D_API::Component
{
public:

	SizeModifierComponent() {
		PM3D_API::EventSystem::Subscribe([this](const CollisionObstacleEvent& event)
			{
				_collisionHappend = true;
			});


		PM3D_API::EventSystem::Subscribe([this](const RestartEvent&)
			{
				_resetRequested = true;
			});

		PM3D_API::EventSystem::Subscribe([this](const InTunnelEvent&)
			{
				_inTunnel = true;
			});
	}

	void PhysicsUpdate() override
	{
		physx::PxShape* shape = parentObject->GetComponent<PM3D_API::SphereCollider>()->getShape();
		if (_resetRequested)
		{
			_resetRequested = false;
			_inTunnel = false;
			shape->setGeometry(physx::PxSphereGeometry(0.2f));
			parentObject->SetWorldPosition(XMFLOAT3(0.f, -60.f, 0.f));
			parentObject->SetWorldScale(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
			return;
		}

		DirectX::XMFLOAT3 preScale = parentObject->GetWorldScale();

		if (_collisionHappend)
		{
			parentObject->SetWorldScale(DirectX::XMFLOAT3(
				preScale.x * 0.5f,
				preScale.y * 0.5f,
				preScale.z * 0.5f
			));
			_collisionHappend = false;
			shape->setGeometry(physx::PxSphereGeometry(preScale.x * .5f));

			if (preScale.x * .5f < 0.1f)
				PM3D_API::EventSystem::Publish(GameOverEvent(false));
		}
		else
		{
			if (preScale.x < 2.f && !_inTunnel) // < 2.f pour les tests, à enlever quand on aura des obstacles
				parentObject->SetWorldScale(DirectX::XMFLOAT3(
					preScale.x * _sizeModificationSpeed,
					preScale.y * _sizeModificationSpeed,
					preScale.z * _sizeModificationSpeed
				));
			else if (_inTunnel)
				parentObject->SetWorldScale(DirectX::XMFLOAT3(
					preScale.x * _sizeModificationSpeedDecrease,
					preScale.y * _sizeModificationSpeedDecrease,
					preScale.z * _sizeModificationSpeedDecrease
				));

			shape->setGeometry(physx::PxSphereGeometry(preScale.x * _sizeModificationSpeed));
		}
		physx::PxFilterData filterDataSnowBall = shape->getSimulationFilterData();
		if (parentObject->GetWorldScale().x >= 0.9f)
			filterDataSnowBall.word2 = BuriablePenguin::eCanBury;
		else
			filterDataSnowBall.word2 = BuriablePenguin::eCannotBury;
		shape->setSimulationFilterData(filterDataSnowBall);


	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Speed");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_sizeModificationSpeed).c_str());
	}

private:
	float _sizeModificationSpeed = 1.002f;
	float _sizeModificationSpeedDecrease = 0.998f;
	bool _collisionHappend = false;
	bool _resetRequested = false;
	bool _inTunnel = false;
};
