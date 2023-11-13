#pragma once
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
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
    void Draw() override;

private:
    bool isPaused = false;
    bool isEnded = false;
    bool isWon = false;

    PM3D_API::SpriteRenderer* pauseRenderer = nullptr;
    PM3D_API::SpriteRenderer* looseRenderer = nullptr;
    PM3D_API::SpriteRenderer* winRenderer = nullptr;
};
