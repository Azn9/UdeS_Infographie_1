#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Imgui/imgui.h"
#include "Component/Component.h"
#include "GameObject/GameObject.h"
#include "Component/Basic/Physics/SphereCollider.h"

class SizeModifierComponent final : public PM3D_API::Component
{
public:
	void Update() override
	{
		DirectX::XMFLOAT3 preScale = parentObject->GetWorldScale();
		parentObject->SetWorldScale(DirectX::XMFLOAT3(
			preScale.x * _sizeModificationSpeed,
			preScale.y * _sizeModificationSpeed,
			preScale.z * _sizeModificationSpeed
		));

		// modify the scale of the collider here
		physx::PxShape* shape = parentObject->GetComponent<PM3D_API::SphereCollider>()->getShape();
		shape->setGeometry(physx::PxSphereGeometry(preScale.x * _sizeModificationSpeed));
	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Speed");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_sizeModificationSpeed).c_str());
	}

private:
	float _sizeModificationSpeed = 1.001f;
};