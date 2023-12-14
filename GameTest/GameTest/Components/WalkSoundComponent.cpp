#include "WalkSoundComponent.h"

#include <iostream>
#include <ostream>
#include <PxScene.h>

#include "SizeModifierComponent.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Scene/Scene.h"
#include "Api/Public/Util/Sound/SoundManager.h"

void WalkSoundComponent::Initialize()
{
    sizeModifierComponent = parentObject->GetComponent<SizeModifierComponent>();

    for (int i = 1; i <= 25; ++i)
    {
        std::string name;
        if (i < 10)
        {
            name = "0" + std::to_string(i);
        }
        else
        {
            name = std::to_string(i);
        }

        const auto res = SoundManager::GetInstance().loadSound("sounds/effects/walk/" + name + ".wav", &buffers[i - 1]);
        if (!res)
        {
            std::cerr << "Failed to load sound: " << name << std::endl;
        }
        else
        {
            std::cout << "Loaded sound: " << name << std::endl;
        }
    }
}

void WalkSoundComponent::PhysicsUpdate()
{
    static int tick;
    ++tick;

    if (tick % 10 != 0) return;
    tick = 0;

    const auto position = parentObject->GetWorldPosition();
    const auto scene = parentObject->GetScene()->GetPhysicsResolver()->GetScene();

    const physx::PxQueryFilterData filterData(physx::PxQueryFlag::eSTATIC);

    if (physx::PxRaycastBuffer hit; scene->raycast(
        physx::PxVec3(position.x, position.y, position.z),
        physx::PxVec3(0.0f, -1.0f, 0.0f),
        sizeModifierComponent->currentSize + 0.2f,
        hit, physx::PxHitFlag::eDEFAULT, filterData
    ))
    {
        const auto index = rand() % 25;
        SoundManager::GetInstance().Play(buffers[index]);
    }
}
