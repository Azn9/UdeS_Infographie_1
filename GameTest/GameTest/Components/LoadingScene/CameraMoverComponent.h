#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Core/Imgui/imgui.h"
#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"

class CameraMoverComponent final : public PM3D_API::Component
{
public:
	void Update() override
	{
		angle += 10.0f * PM3D::Time::GetInstance().GetUpdateDeltaTime();
		if (angle >= 360)
			angle = 0;

		const auto position = parentObject->GetWorldPosition();
		const auto newPosition = DirectX::XMFLOAT3(
			cosf(angle * DirectX::XM_PI / 180.0f) * distance,
			position.y,
			sinf(angle * DirectX::XM_PI / 180.0f) * distance
		);

		parentObject->SetWorldPosition(newPosition);
		static_cast<PM3D_API::Camera*>(parentObject)->SetFocusPoint(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Distance");
		ImGui::SameLine(100); ImGui::Text(std::to_string(distance).c_str());

		ImGui::Text("Angle");
		ImGui::SameLine(100); ImGui::Text(std::to_string(angle).c_str());
	}

private:
	float distance = 15.0f;
	float angle = 0.0f;
};
