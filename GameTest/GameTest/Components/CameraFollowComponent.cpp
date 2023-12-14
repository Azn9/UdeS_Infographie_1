#include "CameraFollowComponent.h"

#include "Api/Public/Camera/Camera.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/InTunnelEvent.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Input/Input.h"
#include "Api/Public/Util/Util.h"
#include "Core/Public/Util/Time.h"
#include "GameTest/RestartEvent.h"

CameraFollowComponent::CameraFollowComponent()
{
    PM3D_API::EventSystem::Subscribe([this](const InTunnelEvent&)
    {
        _inTunnel = true;
    });
    PM3D_API::EventSystem::Subscribe([this](const RestartEvent&)
    {
        _resetRequested = true;
    });
}

void CameraFollowComponent::PhysicsUpdate()
{
    if (!_objectToFollow)
        return;

    if (_resetRequested)
    {
        _resetRequested = false;
        _inTunnel = false;
    }

    if (Input::IsKeyPressed(KeyCode::C)) // Ou IsKeyHeld
    {
        if (camType == CameraType::ThirdPerson)
            camType = CameraType::FirstPerson;
        else if (camType == CameraType::FirstPerson)
            camType = CameraType::FirstPersonWTF;
        else if (camType == CameraType::FirstPersonWTF)
        {
            static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({0.f, 1.f, 0.f, 0.f});
            camType = CameraType::ThirdPerson;
        }
    }

    const DirectX::XMFLOAT3 positionObjectToFollow = _objectToFollow->GetWorldPosition();
    const DirectX::XMFLOAT3 scaleObjectToFollow = _objectToFollow->GetWorldScale();
    const DirectX::XMFLOAT3 rotationObjectToFollow = _objectToFollow->GetWorldRotationEuler();
    _distance = scaleObjectToFollow.x * 20.f;

    const auto rigidbody = _objectToFollow->GetComponent<PM3D_API::Rigidbody>();
    const auto vel = static_cast<physx::PxRigidDynamic*>(rigidbody->GetActor())->getLinearVelocity();

    const auto velNormalized = vel.getNormalized();

    DirectX::XMFLOAT3 position;
    auto camRFP = parentObject->GetChild<PM3D_API::Camera>();

    if (_inTunnel)
    {
        position = DirectX::XMFLOAT3(
            positionObjectToFollow.x - 10 * velNormalized.x,
            positionObjectToFollow.y + 10.0f,
            positionObjectToFollow.z - 10 * velNormalized.z
        );
        position = Util::Lerp(parentObject->GetWorldPosition(), position,
                              PM3D::Time::GetInstance().GetUpdateDeltaTime() * 3.0f);
        static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({0.f, 1.f, 0.f, 0.f});
    }
    else if (camType == CameraType::ThirdPerson)
    {
        position = DirectX::XMFLOAT3(
            positionObjectToFollow.x - 10 * velNormalized.x,
            positionObjectToFollow.y + 10.0f,
            positionObjectToFollow.z - 10 * velNormalized.z
        );
        position = Util::Lerp(parentObject->GetWorldPosition(), position,
                              PM3D::Time::GetInstance().GetUpdateDeltaTime() * 3.0f);
    }
    else if (camType == CameraType::FirstPerson)
    {
        position = DirectX::XMFLOAT3(
            positionObjectToFollow.x,
            positionObjectToFollow.y,
            positionObjectToFollow.z - .5f - scaleObjectToFollow.x
        );
    }
    else if (camType == CameraType::FirstPersonWTF)
    {
        position = DirectX::XMFLOAT3(
            positionObjectToFollow.x,
            positionObjectToFollow.y + cos(_angle) * (scaleObjectToFollow.x * 0.35f),
            positionObjectToFollow.z - sin(_angle) * (scaleObjectToFollow.x * 0.35f)
        );

        if (positionObjectToFollow.z - sin(_angle) * (scaleObjectToFollow.x * 0.5f) <= positionObjectToFollow.z)
            static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({0.f, -1.f, 0.f, 0.f});
        else
            static_cast<PM3D_API::Camera*>(parentObject)->SetUpDirection({0.f, 1.f, 0.f, 0.f});

        const float velMean = sqrtf((vel.x * vel.x) + (vel.y * vel.y) + (vel.z * vel.z));

        _angle += max(velMean / 250.f, 0.05f);
    }

    parentObject->SetWorldPosition(position);

    DirectX::XMFLOAT3 camLookAt;

    if (_inTunnel)
    {
        camLookAt = DirectX::XMFLOAT3(50, -1200, -4500);
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
        camLookAt = DirectX::XMFLOAT3(
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
