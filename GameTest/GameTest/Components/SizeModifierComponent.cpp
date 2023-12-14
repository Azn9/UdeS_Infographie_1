#include "SizeModifierComponent.h"

#include <DirectXMath.h>
#include <PxShape.h>
#include <geometry/PxSphereGeometry.h>

#include "Api/Public/Component/Basic/Physics/SphereCollider.h"
#include "Api/Public/EventSystem/CollisionObstacleEvent.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/InTunnelEvent.h"
#include "Api/Public/GameObject/GameObject.h"
#include "GameTest/GameTest.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Event/GameOverEvent.h"
#include "Api/Public/Util/FilterGroup.h"

SizeModifierComponent::SizeModifierComponent()
{
    PM3D_API::EventSystem::Subscribe([this](const CollisionObstacleEvent& event)
    {
        _collisionHappend = true;
    });


    PM3D_API::EventSystem::Subscribe([this](const RestartEvent&)
    {
        _resetRequested = true;
    });

    PM3D_API::EventSystem::Subscribe([this](const InTunnelEvent&)
    {
        _inTunnel = true;
    });
}

void SizeModifierComponent::PhysicsUpdate()
{
    physx::PxShape* shape = parentObject->GetComponent<PM3D_API::SphereCollider>()->getShape();
    if (_resetRequested)
    {
        _resetRequested = false;
        _inTunnel = false;
        shape->setGeometry(physx::PxSphereGeometry(0.2f));
        parentObject->SetWorldPosition(DirectX::XMFLOAT3(0.f, -60.f, 0.f));
        parentObject->SetWorldScale(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
        return;
    }

    const DirectX::XMFLOAT3 preScale = parentObject->GetWorldScale();

    if (_collisionHappend)
    {
        parentObject->SetWorldScale(DirectX::XMFLOAT3(
            preScale.x * 0.5f,
            preScale.y * 0.5f,
            preScale.z * 0.5f
        ));
        _collisionHappend = false;
        shape->setGeometry(physx::PxSphereGeometry(preScale.x * .5f));

        if (preScale.x * .5f < 0.1f)
            PM3D_API::EventSystem::Publish(GameOverEvent(false));
    }
    else
    {
        if (preScale.x < MAX_SIZE && !_inTunnel)
            parentObject->SetWorldScale(DirectX::XMFLOAT3(
                preScale.x * _sizeModificationSpeed,
                preScale.y * _sizeModificationSpeed,
                preScale.z * _sizeModificationSpeed
            ));
        else if (_inTunnel)
            parentObject->SetWorldScale(DirectX::XMFLOAT3(
                preScale.x * _sizeModificationSpeedDecrease,
                preScale.y * _sizeModificationSpeedDecrease,
                preScale.z * _sizeModificationSpeedDecrease
            ));

        currentSize = preScale.x * _sizeModificationSpeed;

        shape->setGeometry(physx::PxSphereGeometry(currentSize));
    }
    physx::PxFilterData filterDataSnowBall = shape->getSimulationFilterData();
    if (parentObject->GetWorldScale().x >= 0.9f)
        filterDataSnowBall.word2 = BuriablePenguin::eCanBury;
    else
        filterDataSnowBall.word2 = BuriablePenguin::eCannotBury;
    shape->setSimulationFilterData(filterDataSnowBall);
}

void SizeModifierComponent::DrawDebugInfo() const
{
    ImGui::Text("Speed");
    ImGui::SameLine(100);
    ImGui::Text(std::to_string(_sizeModificationSpeed).c_str());
}
