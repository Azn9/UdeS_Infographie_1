#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "../../../PM3DApi/Api/Public/Component/Component.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"

class CameraMoverComponent final : public PM3D_API::Component
{
public:
	void Update(double deltaTime) override
	{
		std::cout << "RotateCameraComponent::Update()" << std::endl;
	
		angle++;
		if (angle >= 360)
			angle = 0;

		const auto position = parentObject->GetWorldPosition();
		const auto newPosition = DirectX::XMFLOAT3(
			cosf(angle * DirectX::XM_PI / 180.0f) * distance,
			position.y,
			sinf(angle * DirectX::XM_PI / 180.0f) * distance
		);

		parentObject->SetWorldPosition(newPosition);
	}

private:
	float distance = 10.0f;
	float angle = 0.0f;
};
