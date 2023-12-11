#pragma once
#include <DirectXMath.h>
#include <iostream>
#include "Core/Imgui/imgui.h"
#include "Api/Public/Input/Input.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include <Api/Public/EventSystem/InTunnelEvent.h>
#include "GameTest/Event/GameOverEvent.h"

#include "GameTest/RestartEvent.h"

enum class CameraType {
		ThirdPerson = 0,
		FirstPerson = 1,
		FirstPersonWTF = 2
	};

class CameraFollowComponent final : public PM3D_API::Component
{
public:
	CameraFollowComponent() {
		PM3D_API::EventSystem::Subscribe([this](const InTunnelEvent&)
			{
				_inTunnel = true;
			});
		PM3D_API::EventSystem::Subscribe([this](const RestartEvent&)
			{
				_resetRequested = true;
			});
	}

	void PhysicsUpdate() override
	{
		if (!_objectToFollow)
			return;
		
		if (_resetRequested) {
			_resetRequested = false;
			_inTunnel = false;
		}

		if (Input::IsKeyPressed(KeyCode::C)) // Ou IsKeyHeld
		{
			if (camType == CameraType::ThirdPerson)
				camType = CameraType::FirstPerson;
			else if (camType == CameraType::FirstPerson)
				camType = CameraType::FirstPersonWTF;
			else if (camType == CameraType::FirstPersonWTF) {
				static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({ 0.f,1.f,0.f,0.f });
				camType = CameraType::ThirdPerson;
			}
		}

		const XMFLOAT3 positionObjectToFollow = _objectToFollow->GetWorldPosition();
		const XMFLOAT3 scaleObjectToFollow = _objectToFollow->GetWorldScale();
		const XMFLOAT3 rotationObjectToFollow = _objectToFollow->GetWorldRotationEuler();
		_distance = scaleObjectToFollow.x * 20.f;

		const auto rigidbody = _objectToFollow->GetComponent<PM3D_API::Rigidbody>();
		const auto vel = static_cast<physx::PxRigidDynamic*>(rigidbody->GetActor())->getLinearVelocity();

		const auto velNormalized = vel.getNormalized();

		DirectX::XMFLOAT3 position;
		auto camRFP = parentObject->GetChild<PM3D_API::Camera>();

		if (_inTunnel) {
			position = XMFLOAT3(
				positionObjectToFollow.x - 10 * velNormalized.x,
				positionObjectToFollow.y + 10.0f,
				positionObjectToFollow.z - 10 * velNormalized.z
			);
			position = Util::Lerp(parentObject->GetWorldPosition(), position, PM3D::Time::GetInstance().GetUpdateDeltaTime() * 3.0f);
			static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({ 0.f,1.f,0.f,0.f });
		}
		else if (camType == CameraType::ThirdPerson)
		{
			position = XMFLOAT3(
				positionObjectToFollow.x - 10 * velNormalized.x,
				positionObjectToFollow.y + 10.0f,
				positionObjectToFollow.z - 10 * velNormalized.z
			);
		}
		else if (camType == CameraType::FirstPerson)
		{
			position = XMFLOAT3(
				positionObjectToFollow.x,
				positionObjectToFollow.y,
				positionObjectToFollow.z - .5f - scaleObjectToFollow.x
			);
		}
		else if (camType == CameraType::FirstPersonWTF)
		{
			position = XMFLOAT3(
				positionObjectToFollow.x,
				positionObjectToFollow.y + cos(_angle) * (scaleObjectToFollow.x * 0.5f) ,
				positionObjectToFollow.z - sin(_angle) * (scaleObjectToFollow.x * 0.5f)
			);

			if (positionObjectToFollow.z - sin(_angle) * (scaleObjectToFollow.x * 0.5f) <= positionObjectToFollow.z)
				static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({ 0.f,-1.f,0.f,0.f });
			else
				static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({ 0.f,1.f,0.f,0.f });

			const float velMean = sqrtf((vel.x * vel.x) + (vel.y * vel.y) + (vel.z * vel.z));

			_angle += max(velMean / 500.f, 0.05f);
		}

		parentObject->SetWorldPosition(position);
		

		DirectX::XMFLOAT3 camLookAt;

		if (_inTunnel) {
			
			camLookAt = XMFLOAT3(50, -1200, -4500);
			camLookAt = Util::Lerp(_currentLookAt, camLookAt, PM3D::Time::GetInstance().GetUpdateDeltaTime() * 0.3f);
		}
		else if (camType == CameraType::ThirdPerson)
		{
			const float velDivider = 5.f;
			DirectX::XMFLOAT3 camLookAt_Temp = DirectX::XMFLOAT3(
				0.f + velNormalized.x / velDivider,
				0.f + velNormalized.y / velDivider,
				-2.f + velNormalized.z / velDivider);
			camLookAt = DirectX::XMFLOAT3(
				positionObjectToFollow.x + camLookAt_Temp.x,
				positionObjectToFollow.y - 1, // + camLookAt.y, 
				positionObjectToFollow.z + camLookAt_Temp.z
			);
		}
		else if (camType == CameraType::FirstPerson)
		{
			const float velDivider = 2.5f;
			DirectX::XMFLOAT3 camLookAt_Temp = DirectX::XMFLOAT3(
				0.f + velNormalized.x / velDivider,
				0.f + velNormalized.y / velDivider,
				-4.f + velNormalized.z / velDivider);
			camLookAt = XMFLOAT3(
				positionObjectToFollow.x + camLookAt_Temp.x,
				positionObjectToFollow.y - 1, // + camLookAt.y, 
				positionObjectToFollow.z + camLookAt_Temp.z
			);
		}
		else if (camType == CameraType::FirstPersonWTF)
		{
			camLookAt = positionObjectToFollow;
		}


		static_cast<PM3D_API::Camera*>(parentObject)->SetFocusPoint(camLookAt);
		_currentLookAt = camLookAt;
	}

	void DrawDebugInfo() const override
	{
		ImGui::Text("Distance");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_distance).c_str());

		ImGui::Text("Angle");
		ImGui::SameLine(100); ImGui::Text(std::to_string(_angle).c_str());
	}

	void SetObjectToFollow(PM3D_API::GameObject* objectToFollow) {
		_objectToFollow = objectToFollow;
	}

private:
	float _distance = 5.0f;
	float _angle = 45.0f;
	PM3D_API::GameObject* _objectToFollow{};
	CameraType camType = CameraType::ThirdPerson;
	bool _inTunnel = false;
	bool _resetRequested = false;
	XMFLOAT3 _currentLookAt{};
};
