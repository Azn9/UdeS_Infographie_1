#include "../../Public/Camera/Camera.h"

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>

#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../Public/GameHost.h"

void PM3D_API::Camera::SetLocalPosition(const DirectX::XMFLOAT3 newPosition)
{
	GameObject::SetLocalPosition(newPosition);
	SetFocusPoint(focusPoint); // Recalculate rotation
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetLocalRotation(const DirectX::XMFLOAT3 newRotation)
{
	GameObject::SetLocalRotation(newRotation);
	SetFocusPoint(focusPoint); // Recalculate rotation
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetWorldPosition(const DirectX::XMFLOAT3 newPosition)
{
	GameObject::SetWorldPosition(newPosition);
	SetFocusPoint(focusPoint); // Recalculate rotation
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetWorldRotation(const DirectX::XMFLOAT3 newRotation)
{
	GameObject::SetWorldRotation(newRotation);
	SetFocusPoint(focusPoint); // Recalculate rotation
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetWorldRotation(const PM3D_API::Quaternion newRotation)
{
	GameObject::SetWorldRotation(newRotation);
	SetFocusPoint(focusPoint); // Recalculate rotation
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetLocalRotation(const PM3D_API::Quaternion newRotation)
{
	GameObject::SetLocalRotation(newRotation);
	SetFocusPoint(focusPoint); // Recalculate rotation
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetFocusPoint(const DirectX::XMFLOAT3 newFocusPoint)
{
	SetFocusPoint(DirectX::XMVectorSet(newFocusPoint.x, newFocusPoint.y, newFocusPoint.z, 1.0f));
}

void PM3D_API::Camera::SetFocusPoint(const DirectX::XMVECTOR newFocusPoint)
{
	focusPoint = newFocusPoint;

	const auto position = GetWorldPosition();

	// Generate a new rotation from the focus point
	const DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationRollPitchYawFromVector(
		DirectX::XMVector3Normalize(
			DirectX::XMVectorSubtract(
				DirectX::XMVectorSet(DirectX::XMVectorGetX(newFocusPoint), DirectX::XMVectorGetY(newFocusPoint),
					DirectX::XMVectorGetZ(newFocusPoint), 1.0f),
				DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f)
			)
		)
	);

	PM3D_API::Quaternion rotation;
	XMStoreFloat4(&rotation, newRotation);

	PM3D_API::GameObject::SetWorldRotation(rotation);
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetUpVector(const DirectX::XMFLOAT3 newUpVector)
{
	upVector = DirectX::XMVectorSet(newUpVector.x, newUpVector.y, newUpVector.z, 1.0f);
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetFieldOfView(const float newFieldOfView)
{
	fieldOfView = newFieldOfView;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetNearPlane(const float newNearPlane)
{
	nearPlane = newNearPlane;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetFarPlane(const float newFarPlane)
{
	farPlane = newFarPlane;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::DrawDebugInfo() const
{
	const float focusPoint[3] = {
		DirectX::XMVectorGetX(this->focusPoint),
		DirectX::XMVectorGetY(this->focusPoint),
		DirectX::XMVectorGetZ(this->focusPoint)
	};
	const float upVector[3] = {
		DirectX::XMVectorGetX(this->upVector),
		DirectX::XMVectorGetY(this->upVector),
		DirectX::XMVectorGetZ(this->upVector)
	};
	
	ImGui::Text("Focus point");
	ImGui::SameLine(100); ImGui::Text(("x=" + std::to_string(focusPoint[0])).c_str());
	ImGui::SameLine(200); ImGui::Text(("y=" + std::to_string(focusPoint[1])).c_str());
	ImGui::SameLine(300); ImGui::Text(("z=" + std::to_string(focusPoint[2])).c_str());

	ImGui::Text("Up vector");
	ImGui::SameLine(100); ImGui::Text(("x=" + std::to_string(upVector[0])).c_str());
	ImGui::SameLine(200); ImGui::Text(("y=" + std::to_string(upVector[1])).c_str());
	ImGui::SameLine(300); ImGui::Text(("z=" + std::to_string(upVector[2])).c_str());

	ImGui::Text("Type");
	ImGui::SameLine(100); ImGui::Text(cameraType == PERSECTIVE ? "Perspective" : "Orthographic");

	if (cameraType == PERSECTIVE)
	{
		ImGui::Text("Field of view");
		ImGui::SameLine(100); ImGui::Text(std::to_string(fieldOfView).c_str());
	}

	ImGui::Text("Near plane");
	ImGui::SameLine(100); ImGui::Text(std::to_string(nearPlane).c_str());

	ImGui::Text("Far plane");
	ImGui::SameLine(100); ImGui::Text(std::to_string(farPlane).c_str());
}

void PM3D_API::Camera::UpdateInternalMatrices()
{
	std::cout << "Camera::UpdateInternalMatrices()" << std::endl;
	const DirectX::XMFLOAT3 position = GetWorldPosition();

	matView = DirectX::XMMatrixLookAtRH(
		DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f),
		focusPoint,
		upVector
	);

	if (cameraType == PERSECTIVE)
	{
		const auto aspectRatio = PM3D_API::GameHost::GetInstance()->GetAspectRatio();

		std::cout << "Camera::UpdateInternalMatrices: aspectRatio=" << aspectRatio << std::endl;

		matProj = DirectX::XMMatrixPerspectiveFovRH(
			fieldOfView,
			aspectRatio,
			nearPlane,
			farPlane
		);
	}
	else // ORTHOGRAPHIC
	{
		matProj = DirectX::XMMatrixOrthographicRH(
			PM3D_API::GameHost::GetInstance()->GetScreenWidth(),
			PM3D_API::GameHost::GetInstance()->GetScreenHeight(),
			nearPlane,
			farPlane
		);
	}

	// Calcul de VP
	matViewProj = matView * matProj;

	initialized = true;
}
