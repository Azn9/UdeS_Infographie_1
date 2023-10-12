#pragma once

#include "../GameObject/GameObject.h"

class Camera final : public PM3D_API::GameObject
{
public:
	enum CameraType
	{
		PERSECTIVE,
		ORTHOGRAPHIC
	};

	Camera(
		const std::string& name,
		const CameraType cameraType,
		const DirectX::XMFLOAT3 worldPosition,
		const DirectX::XMVECTOR focusPoint,
		const DirectX::XMVECTOR upVector
	) : GameObject(
			name,
			worldPosition,
			{0, 0, 0},
			{0, 0, 0}
		), cameraType(cameraType), focusPoint(focusPoint), upVector(upVector)
	{
		SetFocusPoint(focusPoint); // Recalculate rotation
	}

	Camera(
		const CameraType cameraType,
		const DirectX::XMFLOAT3 worldPosition,
		const DirectX::XMVECTOR focusPoint,
		const DirectX::XMVECTOR upVector
	) : GameObject(
			"Main camera",
		    worldPosition,
		    {0, 0, 0},
		    {0, 0, 0}
	    ), cameraType(cameraType), focusPoint(focusPoint), upVector(upVector)
	{
		SetFocusPoint(focusPoint); // Recalculate rotation
	}

	void SetLocalPosition(DirectX::XMFLOAT3 newPosition) override;
	void SetWorldPosition(DirectX::XMFLOAT3 newPosition) override;
	void SetWorldRotation(DirectX::XMFLOAT3 newRotation) override;
	void SetWorldRotation(Quaternion newRotation) override;
	void SetLocalRotation(DirectX::XMFLOAT3 newRotation) override;
	void SetLocalRotation(Quaternion newRotation) override;

	void SetFocusPoint(DirectX::XMFLOAT3 newFocusPoint);
	void SetFocusPoint(DirectX::XMVECTOR newFocusPoint);
	void SetUpVector(DirectX::XMFLOAT3 newUpVector);

	void SetFieldOfView(float newFieldOfView);
	void SetNearPlane(float newNearPlane);
	void SetFarPlane(float newFarPlane);

	const DirectX::XMMATRIX& GetMatView() const { return matView; }
	const DirectX::XMMATRIX& GetMatProj() const { return matProj; }
	const DirectX::XMMATRIX& GetMatViewProj() const { return matViewProj; }

	const DirectX::XMVECTOR& GetFocusPoint() const { return focusPoint; }
	const DirectX::XMVECTOR& GetUpVector() const { return upVector; }

protected:
	CameraType cameraType;

	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matProj;
	DirectX::XMMATRIX matViewProj;

	float fieldOfView = 45.0f;
	float nearPlane = 0.05f;
	float farPlane = 400.0f;

	DirectX::XMVECTOR focusPoint;
	DirectX::XMVECTOR upVector;

private:
	void UpdateInternalMatrices();
};
