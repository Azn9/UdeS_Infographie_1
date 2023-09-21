#pragma once

#include "../GameObject/GameObject.h"

class Camera final : public PM3D_API::GameObject
{
public:
	Camera(
		const DirectX::XMFLOAT3 position,
		const DirectX::XMFLOAT3 rotation
	) : GameObject(position, rotation, {0, 0, 0})
	{
		UpdateInternalMatrices();
	}

	Camera(
	) : GameObject(
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	)
	{
		UpdateInternalMatrices();
	}

	void SetPosition(DirectX::XMFLOAT3 newPosition) override;
	void SetRotation(DirectX::XMFLOAT3 newRotation) override;

	void LerpPosition(DirectX::XMFLOAT3 newPosition, float t);
	void LerpRotation(DirectX::XMFLOAT3 newRotation, float t);
	void SetFocusPoint(DirectX::XMFLOAT3 newFocusPoint);

	void SetFieldOfView(float newFieldOfView);
	void SetNearPlane(float newNearPlane);
	void SetFarPlane(float newFarPlane);

	const DirectX::XMMATRIX& GetMatView() const { return MatView; }
	const DirectX::XMMATRIX& GetMatProj() const { return MatProj; }
	const DirectX::XMMATRIX& GetMatViewProj() const { return MatViewProj; }

protected:
	DirectX::XMMATRIX MatView;
	DirectX::XMMATRIX MatProj;
	DirectX::XMMATRIX MatViewProj;

	float FieldOfView = 45.0f;
	float NearPlane = 0.05f;
	float FarPlane = 400.0f;

private:
	void UpdateInternalMatrices();
};
