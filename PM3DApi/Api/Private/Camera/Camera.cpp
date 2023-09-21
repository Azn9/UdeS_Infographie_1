#include "Api/Public/Camera/Camera.h"

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>

#include "Api/Public/GameHost.h"
#include "Api/Public/Util/Util.h"


void Camera::SetPosition(const DirectX::XMFLOAT3 newPosition)
{
	GameObject::SetPosition(newPosition);

	UpdateInternalMatrices();
}

void Camera::SetRotation(const DirectX::XMFLOAT3 newRotation)
{
	GameObject::SetRotation(newRotation);

	UpdateInternalMatrices();
}

void Camera::LerpPosition(const DirectX::XMFLOAT3 newPosition, const float t)
{
	SetPosition(Util::Lerp(GetPosition(), newPosition, t));
}

void Camera::LerpRotation(const DirectX::XMFLOAT3 newRotation, const float t)
{
	SetRotation(Util::Lerp(GetRotation(), newRotation, t));
}

void Camera::SetFocusPoint(const DirectX::XMFLOAT3 newFocusPoint)
{
	const auto position = GetPosition();

	// Generate a new rotation from the focus point
	const DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationRollPitchYawFromVector(
		DirectX::XMVector3Normalize(
			DirectX::XMVectorSubtract(
				DirectX::XMVectorSet(newFocusPoint.x, newFocusPoint.y, newFocusPoint.z, 1.0f),
				DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f)
			)
		)
	);

	// Set the new rotation
	const auto newRotationFloat4 = DirectX::XMFLOAT4(
		DirectX::XMVectorGetX(newRotation),
		DirectX::XMVectorGetY(newRotation),
		DirectX::XMVectorGetZ(newRotation),
		DirectX::XMVectorGetW(newRotation)
	);

	SetRotation(Util::QuaternionToEuler(newRotationFloat4));
}

void Camera::SetFieldOfView(const float newFieldOfView)
{
	FieldOfView = newFieldOfView;
	UpdateInternalMatrices();
}

void Camera::SetNearPlane(const float newNearPlane)
{
	NearPlane = newNearPlane;
	UpdateInternalMatrices();
}

void Camera::SetFarPlane(const float newFarPlane)
{
	FarPlane = newFarPlane;
	UpdateInternalMatrices();
}

void Camera::UpdateInternalMatrices()
{
	const DirectX::XMFLOAT3 position = GetPosition();
	const DirectX::XMFLOAT3 rotation = GetRotation();

	// Convert rotation to rotated up and forward vectors
	const DirectX::XMVECTOR forward = DirectX::XMVector3Rotate(
		DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation))
	);
	const DirectX::XMVECTOR up = DirectX::XMVector3Rotate(
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation))
	);

	// Focus point is the camera's position + forward vector
	const DirectX::XMVECTOR focusPoint = DirectX::XMVectorAdd(
		DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f),
		forward
	);

	MatView = DirectX::XMMatrixLookAtRH(
		DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f),
		focusPoint,
		up
	);

	const auto aspectRatio = PM3D_API::GameHost::GetInstance()->GetAspectRatio();

	MatProj = DirectX::XMMatrixPerspectiveFovRH(
		FieldOfView,
		aspectRatio,
		NearPlane,
		FarPlane
	);

	// Calcul de VP
	MatViewProj = MatView * MatProj;
}
