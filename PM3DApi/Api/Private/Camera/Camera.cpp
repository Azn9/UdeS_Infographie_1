#include "Api/Public/Camera/Camera.h"

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>

#include "Api/Public/GameHost.h"

void Camera::SetLocalPosition(const DirectX::XMFLOAT3 newPosition)
{
	GameObject::SetLocalPosition(newPosition);
	UpdateInternalMatrices();
}

void Camera::SetLocalRotation(const DirectX::XMFLOAT3 newRotation)
{
	GameObject::SetLocalRotation(newRotation);
	UpdateInternalMatrices();
}

void Camera::SetWorldPosition(const DirectX::XMFLOAT3 newPosition)
{
	GameObject::SetWorldPosition(newPosition);
	UpdateInternalMatrices();
}

void Camera::SetWorldRotation(const DirectX::XMFLOAT3 newRotation)
{
	GameObject::SetWorldRotation(newRotation);
	UpdateInternalMatrices();
}

void Camera::SetWorldRotation(const Quaternion newRotation)
{
	GameObject::SetWorldRotation(newRotation);
	UpdateInternalMatrices();
}

void Camera::SetLocalRotation(const Quaternion newRotation)
{
	GameObject::SetLocalRotation(newRotation);
	UpdateInternalMatrices();
}

void Camera::SetFocusPoint(const DirectX::XMFLOAT3 newFocusPoint)
{
	SetFocusPoint(DirectX::XMVectorSet(newFocusPoint.x, newFocusPoint.y, newFocusPoint.z, 1.0f));
}

void Camera::SetFocusPoint(const DirectX::XMVECTOR newFocusPoint)
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

	Quaternion rotation;
	XMStoreFloat4(&rotation, newRotation);

	SetWorldRotation(rotation);

	UpdateInternalMatrices();
}

void Camera::SetUpVector(const DirectX::XMFLOAT3 newUpVector)
{
	upVector = DirectX::XMVectorSet(newUpVector.x, newUpVector.y, newUpVector.z, 1.0f);
	UpdateInternalMatrices();
}

void Camera::SetFieldOfView(const float newFieldOfView)
{
	fieldOfView = newFieldOfView;
	UpdateInternalMatrices();
}

void Camera::SetNearPlane(const float newNearPlane)
{
	nearPlane = newNearPlane;
	UpdateInternalMatrices();
}

void Camera::SetFarPlane(const float newFarPlane)
{
	farPlane = newFarPlane;
	UpdateInternalMatrices();
}

void Camera::UpdateInternalMatrices()
{
	const DirectX::XMFLOAT3 position = GetWorldPosition();
	const DirectX::XMFLOAT3 rotation = GetWorldRotationEuler();

	matView = DirectX::XMMatrixLookAtRH(
		DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f),
		focusPoint,
		upVector
	);

	if (cameraType == PERSECTIVE)
	{
		const auto aspectRatio = PM3D_API::GameHost::GetInstance()->GetAspectRatio();

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
}
