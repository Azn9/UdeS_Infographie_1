#pragma once
#include <DirectXMath.h>

#include "Api/Public/Component/Component.h"

class TournerComponent final : public PM3D_API::Component
{
public:
    void Initialize() override;
    void Update() override;

private:
    DirectX::XMFLOAT3 originalPosition;
    int angle;
};
