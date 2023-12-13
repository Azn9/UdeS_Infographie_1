#pragma once
#include "Api/Public/EventSystem/Event.h"

struct PauseEvent final : PM3D_API::Event
{
    explicit PauseEvent(const bool paused) : paused(paused)
    {
    }

    bool paused;
};
