#pragma once

#include "Api/Public/Component/Component.h"

class CameraMoverComponent final : public PM3D_API::Component
{
public:
    void Update() override;

    void DrawDebugInfo() const override;

private:
    float distance = 15.0f;
    float angle = 0.0f;
};
