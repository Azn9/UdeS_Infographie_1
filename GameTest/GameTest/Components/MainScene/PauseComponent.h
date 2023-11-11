#pragma once
#include "Api/Public/GameObject/UIObject.h"

class PauseComponent : public PM3D_API::UIObject
{
public:
    PauseComponent() : UIObject(
        "Pause",
        DirectX::XMFLOAT2(1.f, 1.f),
        DirectX::XMFLOAT2(0.f, 0.f),
        0.f,
        true,
        false
    )
    {
        alpha = 0.f;
    }

    void Initialize() override;
    void Update() override;

private:
    bool isPaused = false;
};
