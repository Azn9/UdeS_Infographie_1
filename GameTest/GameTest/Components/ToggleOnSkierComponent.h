#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Core/Imgui/imgui.h"
#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Component/Basic/Physics/SkierCollider.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/CollisionSkierEvent.h"
#include "GameTest/Event/GameOverEvent.h"
#include <Api/Public/EventSystem/InTunnelEvent.h>
#include <Api/Public/Component/Basic/Physics/Rigidbody.h>
#include <Api/Public/EventSystem/CheckpointEvents.h>

class ToggleOnSkierComponent final : public PM3D_API::Component
{
public:
    ToggleOnSkierComponent(int checkpoint)
    {
        if (!_toggleOn)
            switch (checkpoint)
            {
            case 1:
                PM3D_API::EventSystem::Subscribe([this](const CheckpointEvent_1& event)
                {
                    _toggleOn = true;
                });
            case 2:
                PM3D_API::EventSystem::Subscribe([this](const CheckpointEvent_2& event)
                {
                    _toggleOn = true;
                });
            case 3:
                PM3D_API::EventSystem::Subscribe([this](const CheckpointEvent_3& event)
                {
                    _toggleOn = true;
                });
            case 4:
                PM3D_API::EventSystem::Subscribe([this](const CheckpointEvent_4& event)
                {
                    _toggleOn = true;
                });
            case 5:
                PM3D_API::EventSystem::Subscribe([this](const CheckpointEvent_5& event)
                {
                    _toggleOn = true;
                });
            }
    }

    void PhysicsUpdate() override
    {
        if (_toggleOn)
        {
            parentObject->GetComponent<PM3D_API::Rigidbody>()->getRigidDynamic()->setActorFlag(
                physx::PxActorFlag::eDISABLE_GRAVITY, false);
            parentObject->GetComponent<PM3D_API::Rigidbody>()->getRigidDynamic()->wakeUp();
        }
    }

private:
    bool _toggleOn = false;
};
