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
		physx::PxShape* shape = parentObject->GetComponent<PM3D_API::SphereCollider>()->getShape();
		physx::PxShapeFlags flags = shape->getFlags(); // might be put in the if condition
		if (flags & physx::PxShapeFlag::eTRIGGER_SHAPE)
		{
			preScale.x = preScale.x / 2.f;
			preScale.y = preScale.y / 2.f;
			preScale.z = preScale.z / 2.f;
			flags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
			shape->setFlags(flags);
		}

		parentObject->SetWorldScale(DirectX::XMFLOAT3(
			preScale.x * _sizeModificationSpeed,
			preScale.y * _sizeModificationSpeed,
			preScale.z * _sizeModificationSpeed
		));

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