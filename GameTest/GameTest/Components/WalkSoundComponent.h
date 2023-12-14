#pragma once
#include <dsound.h>

#include "SizeModifierComponent.h"
#include "Api/Public/Component/Component.h"

class WalkSoundComponent final : public PM3D_API::Component
{
public:
    WalkSoundComponent() = default;

    void Initialize() override;
    void PhysicsUpdate() override;

private:
    IDirectSoundBuffer8* buffers[25];
    SizeModifierComponent* sizeModifierComponent;
};
