#pragma once
#include <DirectXMath.h>

#include "../../../PM3DApi/Api/Public/Component/Component.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"

class LightMoverComponent : public PM3D_API::Component
{
public:
    explicit LightMoverComponent(const int offset) : angle(offset)
    {
        
    }

    void Update() override
    {
        angle++;
        if (angle >= 360) angle = 0;

        float x = sinf(static_cast<float>(angle) * DirectX::XM_2PI / 360.0f) * 5.0f;
        float z = cosf(static_cast<float>(angle) * DirectX::XM_2PI / 360.0f) * 5.0f;

        const auto worldPosition = parentObject->GetWorldPosition();
        const DirectX::XMFLOAT3 newWorldPosition = {x, worldPosition.y, z};

        parentObject->SetWorldPosition(newWorldPosition);
    }

private:
    int angle;
};
