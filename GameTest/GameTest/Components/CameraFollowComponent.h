#pragma once
#include <DirectXMath.h>
#include <iostream>
#include "Api/Public/Input/Input.h"

class CameraFollowComponent final : public PM3D_API::Component
{
private:
	enum cameraType : int {
		ThirdPerson = 0,
		FirstPerson = 1,
		FirstPersonWTF = 2
	};

public:
	void PhysicsUpdate() override
	{
		if (!_objectToFollow)
			return;
		
		if (Input::IsKeyPressed(KeyCode::C)) // Ou IsKeyHeld
		{
			if (camType == ThirdPerson) 
				camType = FirstPerson;
			else if (camType == FirstPerson) 
				camType = FirstPersonWTF;
			else if (camType == FirstPersonWTF) {
				static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({ 0.f,1.f,0.f,0.f });
				camType = ThirdPerson;
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

		if (camType == ThirdPerson) 
		{
			position = XMFLOAT3(
				positionObjectToFollow.x - 10 * velNormalized.x,
				positionObjectToFollow.y + 10.0f,
				positionObjectToFollow.z - 10 * velNormalized.z
			);
		}
		else if (camType == FirstPerson)
		{
			position = XMFLOAT3(
				positionObjectToFollow.x,
				positionObjectToFollow.y,
				positionObjectToFollow.z - .5f - scaleObjectToFollow.x
			);
		}
		else if (camType == FirstPersonWTF)
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
		
		//const auto finalPosition = Util::Lerp(positionObjectToFollow, position, PM3D::Time::GetInstance().GetUpdateDeltaTime() * 1.0f);



		DirectX::XMFLOAT3 camLookAt;

		if (camType == ThirdPerson)
		{
			const float velDivider = 5.f;
			camLookAt = DirectX::XMFLOAT3(
				0.f + velNormalized.x / velDivider,
				0.f + velNormalized.y / velDivider,
				-2.f + velNormalized.z / velDivider);
			static_cast<PM3D_API::Camera*>(parentObject)->SetFocusPoint(XMFLOAT3(
				positionObjectToFollow.x + camLookAt.x,
				positionObjectToFollow.y - 1, // + camLookAt.y, 
				positionObjectToFollow.z + camLookAt.z
			));
		}
		else if (camType == FirstPerson)
		{
			const float velDivider = 2.5f;
			camLookAt = DirectX::XMFLOAT3(
				0.f + velNormalized.x / velDivider,
				0.f + velNormalized.y / velDivider,
				-4.f + velNormalized.z / velDivider);
			static_cast<PM3D_API::Camera*>(parentObject)->SetFocusPoint(XMFLOAT3(
				positionObjectToFollow.x + camLookAt.x,
				positionObjectToFollow.y - 1, // + camLookAt.y, 
				positionObjectToFollow.z + camLookAt.z
			));
		}
		else if (camType == FirstPersonWTF)
		{
			const float velDivider = 2.5f;
			camLookAt = DirectX::XMFLOAT3(
				0.f + velNormalized.x / velDivider,
				0.f + velNormalized.y / velDivider,
				-4.f + velNormalized.z / velDivider);
			static_cast<PM3D_API::Camera*>(parentObject)->SetFocusPoint(positionObjectToFollow);
		}


		
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
	cameraType camType = ThirdPerson;
};
