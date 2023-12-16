#pragma once

#include "Api/Public/Debug/DebugRenderer.h"

class TimeScaleTest final : public PM3D_API::DebugRenderer
{
public:
    TimeScaleTest() : DebugRenderer()
    {
    }

    ~TimeScaleTest() override = default;

    void StepOneFrame();
    void Draw() override;
};
