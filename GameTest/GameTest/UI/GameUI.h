#pragma once
#include "Api/Public/GameObject/UIObject.h"
#include "GameTest/Components/MainScene/PauseComponent.h"

class GameUI final : public PM3D_API::UIObject
{
public:
    GameUI() : UIObject(
        "GameUI",
        DirectX::XMFLOAT2(1.0f, 1.0f),
        DirectX::XMFLOAT2(0.0f, 0.0f),
        0.0f,
        true,
        true
    )
    {
    }

    void Initialize() override;
    void Update() override;
    std::unique_ptr<PauseComponent>::pointer pauseComponentPtr = nullptr;
};
