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
			PERSPECTIVE,
			ORTHOGRAPHIC
		};

		Camera(
			const std::string& name,
			const CameraType cameraType,
			const DirectX::XMFLOAT3 worldPosition,
			const DirectX::XMVECTOR focusPoint,
			const DirectX::XMVECTOR upDirection = {0.f,1.f,0.f,0.f}
		) : GameObject(
				name,
				worldPosition,
				{0.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 1.0f}
			), cameraType(cameraType), focusPoint(focusPoint), upDirection(upDirection)
		{
			SetFocusPoint(focusPoint); // Recalculate rotation
		}

		Camera(
			const CameraType cameraType,
			const DirectX::XMFLOAT3 worldPosition,
			const DirectX::XMVECTOR focusPoint,
			const DirectX::XMVECTOR upDirection = {0.f,1.f,0.f,0.f}
		) : GameObject(
				"Main camera",
				worldPosition,
				{0.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 1.0f}
			), cameraType(cameraType), focusPoint(focusPoint), upDirection(upDirection)
		{
			SetFocusPoint(focusPoint); // Recalculate rotation
		}

		void Initialize() override;

		void SetLocalPosition(DirectX::XMFLOAT3 newPosition) override;
		void SetWorldPosition(DirectX::XMFLOAT3 newPosition) override;
		void SetWorldRotation(DirectX::XMFLOAT3 newRotation) override;
		void SetWorldRotation(PM3D_API::Quaternion newRotation) override;
		void SetLocalRotation(DirectX::XMFLOAT3 newRotation) override;
		void SetLocalRotation(PM3D_API::Quaternion newRotation) override;

		void SetFocusPoint(DirectX::XMFLOAT3 newFocusPoint);
		void SetFocusPoint(DirectX::XMVECTOR newFocusPoint);
		void SetUpDirection(DirectX::XMVECTOR newUpDirection);

		void SetFieldOfView(float newFieldOfView);
		void SetViewHeight(float newViewHeight);
		void SetViewWidth(float newViewWidth);
		void SetFarDist(float newFarDist);
		void SetNearDist(float newNearDist);

		const DirectX::XMMATRIX& GetMatView() const { return matView; }
		const DirectX::XMMATRIX& GetMatProj() const { return matProj; }
		const DirectX::XMMATRIX& GetMatViewProj() const { return matViewProj; }

		const DirectX::XMVECTOR& GetFocusPoint() const { return focusPoint; }

		const Frustrum& getFrustrum() const{return frustrum;}

		float getFieldOfView() const { return fieldOfView; }
		float getViewHeight() const { return viewHeight; }
		float getViewWidth() const { return viewWidth; }

		float getNearDist() const { return nearDist; }

		float getFarDist() const { return farDist; }

		void DrawDebugInfo() const override;

		void UpdateInternalMatrices();

		bool IsInitialized() const { return initialized; }

		void SetClearColor(const DirectX::XMFLOAT3& newClearColor) { clearColor = newClearColor; }
		const DirectX::XMFLOAT3& GetClearColor() const { return clearColor; }
	protected:
		DirectX::XMFLOAT3 clearColor = {0.0f, 0.0f, 0.0f};
		
		bool initialized = false;
		CameraType cameraType;

		DirectX::XMMATRIX matView;
		DirectX::XMMATRIX matProj;
		DirectX::XMMATRIX matViewProj;

		float fieldOfView = 45.0f;
		float viewHeight = 512.0f;
		float viewWidth = 512.0f;
		
		float nearDist = 0.05f;
		float farDist = 400.0f;

#pragma warning(push)
#pragma warning(disable: 4324)
		DirectX::XMVECTOR focusPoint;

		DirectX::XMVECTOR upDirection;
		Frustrum frustrum;
#pragma warning(pop)

	private:
		//void WindowResizeEventListener(WindowResizeEvent& event);
	};
}
