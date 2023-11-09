#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Imgui/imgui.h"
#include "Component/Component.h"
#include "GameObject/GameObject.h"
#include "Util/Util.h"

class CameraFollowComponent final : public PM3D_API::Component
{
public:
	void Update() override
	{
		if (!_objectToFollow)
			return;
		
		const auto positionObjectToFollow = _objectToFollow->GetWorldPosition();
		const auto scaleObjectToFollow = _objectToFollow->GetWorldScale();
		_distance = 10.f;//scaleObjectToFollow.x * 20.f;

		const auto rigidbody = _objectToFollow->GetComponent<PM3D_API::Rigidbody>();
		const auto vel = static_cast<physx::PxRigidDynamic*>(rigidbody->GetActor())->getLinearVelocity();

		const auto velNormalized = vel.getNormalized();

		const auto position = XMFLOAT3(
			positionObjectToFollow.x - 10 * velNormalized.x,
			positionObjectToFollow.y - 10 * velNormalized.y + 5.0f,
			positionObjectToFollow.z - 10 * velNormalized.z
		);
		const auto finalPosition = Util::Lerp(parentObject->GetWorldPosition(), position, PM3D::Time::GetInstance().GetUpdateDeltaTime() * 3.0f);

		parentObject->SetWorldPosition(finalPosition);
		static_cast<PM3D_API::Camera*>(parentObject)->SetFocusPoint(positionObjectToFollow);
	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Distance");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_distance).c_str());

		ImGui::Text("Angle");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_angle).c_str());
	}

	void SetObjectToFollow(PM3D_API::GameObject* objectToFollow) {
		_objectToFollow = objectToFollow;
	}

private:
	float _distance = 5.0f;
	float _angle = 45.0f;
	PM3D_API::GameObject* _objectToFollow{};
};
