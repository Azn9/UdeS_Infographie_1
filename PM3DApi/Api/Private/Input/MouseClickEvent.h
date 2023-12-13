#pragma once
#include "Api/Public/EventSystem/Event.h"

class MouseClickEvent final : public PM3D_API::Event
{
public:
    MouseClickEvent(
        const int x,
        const int y,
        const bool left,
        const bool pressed
    ) : x(x), y(y), left(left), pressed(pressed)
    {
    }

    int x;
    int y;
    bool left;
    bool pressed;
};
