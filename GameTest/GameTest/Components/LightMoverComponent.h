#pragma once
#include <DirectXMath.h>

#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"

class LightMoverComponent : public PM3D_API::Component
{
public:
    explicit LightMoverComponent(const float offset) : angle(offset)
    {
        
    }

    void Update() override
    {
        angle += 100.0f * PM3D::Time::GetInstance().GetUpdateDeltaTime();
        if (angle >= 360) angle = 0;

        float x = sinf(angle * DirectX::XM_2PI / 360.0f) * 5.0f;
        float z = cosf(angle * DirectX::XM_2PI / 360.0f) * 5.0f;

        const auto worldPosition = parentObject->GetWorldPosition();
        const DirectX::XMFLOAT3 newWorldPosition = {x, worldPosition.y, z};

        parentObject->SetWorldPosition(newWorldPosition);
    }

private:
    float angle;
};
