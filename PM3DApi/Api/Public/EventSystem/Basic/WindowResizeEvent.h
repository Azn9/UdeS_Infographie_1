#pragma once
#include "../Event.h"

namespace PM3D_API
{
    class WindowResizeEvent final : public Event
    {
    public:
        WindowResizeEvent(const int width, const int height) : Event(), width(width), height(height)
        {
        }

        [[nodiscard]] int GetWidth() const { return width; }
        [[nodiscard]] int GetHeight() const { return height; }

    private:
        int width;
        int height;
    };
}
