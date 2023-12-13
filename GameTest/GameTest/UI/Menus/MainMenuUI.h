#pragma once
#include "Api/Public/GameObject/UIObject.h"

class MainMenuUI final : public PM3D_API::UIObject
{
public:
    MainMenuUI() : UIObject(
        "MainMenuUI",
        DirectX::XMFLOAT2(1.0f, 1.0f),
        DirectX::XMFLOAT2(0.0f, 0.0f),
        0.0f,
        true,
        true
    )
    {
    }

    void Initialize() override;
    void DrawSelf() const override;
};
