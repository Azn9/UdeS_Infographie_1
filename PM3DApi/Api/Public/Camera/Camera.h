#pragma once

#include "Frustrum.h"
#include "../GameObject/GameObject.h"

namespace PM3D_API
{
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
				{0.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 1.0f}
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
				{0.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 1.0f}
			), cameraType(cameraType), focusPoint(focusPoint), upVector(upVector)
		{
			SetFocusPoint(focusPoint); // Recalculate rotation
		}

		void SetLocalPosition(DirectX::XMFLOAT3 newPosition) override;
		void SetWorldPosition(DirectX::XMFLOAT3 newPosition) override;
		void SetWorldRotation(DirectX::XMFLOAT3 newRotation) override;
		void SetWorldRotation(PM3D_API::Quaternion newRotation) override;
		void SetLocalRotation(DirectX::XMFLOAT3 newRotation) override;
		void SetLocalRotation(PM3D_API::Quaternion newRotation) override;

		void SetFocusPoint(DirectX::XMFLOAT3 newFocusPoint);
		void SetFocusPoint(DirectX::XMVECTOR newFocusPoint);
		void SetUpVector(DirectX::XMFLOAT3 newUpVector);

		void SetFieldOfView(float newFieldOfView);
		void SetFarDist(float newFarDist);
		void SetNearDist(float newNearDist);

		const DirectX::XMMATRIX& GetMatView() const { return matView; }
		const DirectX::XMMATRIX& GetMatProj() const { return matProj; }
		const DirectX::XMMATRIX& GetMatViewProj() const { return matViewProj; }

		const DirectX::XMVECTOR& GetFocusPoint() const { return focusPoint; }
		const DirectX::XMVECTOR& GetUpVector() const { return upVector; }
		const DirectX::XMVECTOR& GetForwardVector() const { return forwardVector; }
		const DirectX::XMVECTOR& GetRightVector() const { return rightVector; }

		const Frustrum& getFrustrum() const{return frustrum;}

		float getFieldOfView() const { return fieldOfView; }

		float getNearDist() const { return nearDist; }

		float getFarDist() const { return farDist; }

		void DrawDebugInfo() const override;

		void UpdateInternalMatrices();

		bool IsInitialized() const { return initialized; }
	protected:
		bool initialized = false;
		CameraType cameraType;

		DirectX::XMMATRIX matView;
		DirectX::XMMATRIX matProj;
		DirectX::XMMATRIX matViewProj;

		float fieldOfView = 45.0f;
		float nearDist = 0.05f;
		float farDist = 400.0f;

		DirectX::XMVECTOR focusPoint;
		DirectX::XMVECTOR upVector;
		DirectX::XMVECTOR forwardVector;
		DirectX::XMVECTOR rightVector;

		Frustrum frustrum;

	};
}