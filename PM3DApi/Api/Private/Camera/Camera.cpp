#include "Api/Public/Camera/Camera.h"

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>

#include "Core/Imgui/imgui.h"
#include "Api/Public/GameHost.h"
//#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/Basic/WindowResizeEvent.h"
#include "Api/Public/Util/Util.h"

void PM3D_API::Camera::Initialize()
{
	//PM3D_API::EventSystem::Subscribe([](const PM3D_API::WindowResizeEvent& event) {});
}

using namespace Util;

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

	//const auto position = GetWorldPosition();

	UpdateInternalMatrices();
	
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationMatrix(matWorld);
	PM3D_API::GameObject::SetWorldRotation({
		rotation.m128_f32[0],
		rotation.m128_f32[1],
		rotation.m128_f32[2],
		rotation.m128_f32[3]});
}

void PM3D_API::Camera::SetFieldOfView(const float newFieldOfView)
{
	fieldOfView = newFieldOfView;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetViewHeight(const float newViewHeight)
{
	viewHeight = newViewHeight;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetViewWidth(const float newViewWidth)
{
	viewWidth = newViewWidth;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetNearDist(const float newnearDist)
{
	nearDist = newnearDist;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::SetFarDist(const float newfarDist)
{
	farDist = newfarDist;
	UpdateInternalMatrices();
}

void PM3D_API::Camera::DrawDebugInfo() const
{
	const float focusPoint[3] = {
		DirectX::XMVectorGetX(this->focusPoint),
		DirectX::XMVectorGetY(this->focusPoint),
		DirectX::XMVectorGetZ(this->focusPoint)
	};
	
	ImGui::Text("Focus point");
	ImGui::SameLine(100); ImGui::Text(("x=" + std::to_string(focusPoint[0])).c_str());
	ImGui::SameLine(200); ImGui::Text(("y=" + std::to_string(focusPoint[1])).c_str());
	ImGui::SameLine(300); ImGui::Text(("z=" + std::to_string(focusPoint[2])).c_str());

	/*ImGui::Text("Forward vector");
	ImGui::SameLine(100); ImGui::Text(("x=" + std::to_string(forwardVector.m128_f32[0])).c_str());
	ImGui::SameLine(200); ImGui::Text(("y=" + std::to_string(forwardVector.m128_f32[1])).c_str());
	ImGui::SameLine(300); ImGui::Text(("z=" + std::to_string(forwardVector.m128_f32[2])).c_str());*/

	ImGui::Text("Up direction");
	ImGui::SameLine(100); ImGui::Text(("x=" + std::to_string(upDirection.m128_f32[0])).c_str());
	ImGui::SameLine(200); ImGui::Text(("y=" + std::to_string(upDirection.m128_f32[1])).c_str());
	ImGui::SameLine(300); ImGui::Text(("z=" + std::to_string(upDirection.m128_f32[2])).c_str());

	/*ImGui::Text("Right vector");
	ImGui::SameLine(100); ImGui::Text(("x=" + std::to_string(rightVector.m128_f32[0])).c_str());
	ImGui::SameLine(200); ImGui::Text(("y=" + std::to_string(rightVector.m128_f32[1])).c_str());
	ImGui::SameLine(300); ImGui::Text(("z=" + std::to_string(rightVector.m128_f32[2])).c_str());*/
	
	//ImGui::Text("Frustrum");
	//frustrum.DrawDebugInfo();

	ImGui::Text("Type");
	ImGui::SameLine(100); ImGui::Text(cameraType == PERSPECTIVE ? "Perspective" : "Orthographic");

	if (cameraType == PERSPECTIVE)
	{
		ImGui::Text("Field of view");
		ImGui::SameLine(100); ImGui::Text(std::to_string(fieldOfView).c_str());
	}

	ImGui::Text("Near distance");
	ImGui::SameLine(100); ImGui::Text(std::to_string(nearDist).c_str());

	ImGui::Text("Far distance");
	ImGui::SameLine(100); ImGui::Text(std::to_string(farDist).c_str());
}

void PM3D_API::Camera::UpdateInternalMatrices()
{
	const DirectX::XMFLOAT3 position = GetWorldPosition();

	DirectX::XMVECTOR eye_position = DirectX::XMVectorSet(position.x, position.y, position.z, 1.f);
	matView = DirectX::XMMatrixLookAtRH(eye_position, focusPoint, upDirection);
	matWorld = DirectX::XMMatrixInverse(nullptr, matView);

	//forwardVector = DirectX::XMVector3TransformCoord(...);
	
	frustrum.SetPlanes(*this);
	
	if (cameraType == PERSPECTIVE)
	{
		const auto aspectRatio = PM3D_API::GameHost::GetInstance()->GetAspectRatio();
		
		matProj = DirectX::XMMatrixPerspectiveFovRH(
			fieldOfView,
			aspectRatio,
			nearDist,
			farDist
		);
	}
	else // ORTHOGRAPHIC
	{
		matProj = DirectX::XMMatrixOrthographicRH(
			viewWidth,
			viewHeight,
			nearDist,
			farDist
		);
	}

	// Calcul de VP
	matViewProj = matView * matProj;

	initialized = true;
}

void PM3D_API::Camera::SetUpDirection(DirectX::XMVECTOR newUpDirection)
{
	upDirection = newUpDirection;
}
