#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Imgui/imgui.h"
#include "Component/Component.h"
#include "GameObject/GameObject.h"

class CameraFollowComponent final : public PM3D_API::Component
{
public:
	void Update() override
	{
		if (!_objectToFollow)
			return;
		
		const auto positionObjectToFollow = _objectToFollow->GetWorldPosition();

		const auto position = XMFLOAT3(
			positionObjectToFollow.x,
			positionObjectToFollow.y + (sinf(_angle * DirectX::XM_PI / 180.0f) * _distance),
			positionObjectToFollow.z - (cosf(_angle * DirectX::XM_PI / 180.0f) * _distance));

		parentObject->SetWorldPosition(position);
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
	PM3D_API::GameObject* _objectToFollow;
};
