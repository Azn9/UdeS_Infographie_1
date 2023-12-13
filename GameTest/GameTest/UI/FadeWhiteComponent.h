#pragma once
#include "Api/Public/GameObject/UIObject.h"

class FadeWhiteComponent final : public PM3D_API::UIObject
{
public:
    FadeWhiteComponent() : UIObject(
        "FadeWhiteComponent",
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

    void SetFading(const bool isFading)
    {
        fading = isFading;
    }

    void SetToTransparent(const bool isTransparent)
    {
        toTransparent = isTransparent;
    }

    void SetSpeed(const float newSpeed)
    {
        speed = newSpeed;
    }

private:
    bool fading = false;
    bool toTransparent = true;
    float speed = 1.0f;
};
