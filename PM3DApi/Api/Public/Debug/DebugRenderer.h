#pragma once

namespace PM3D_API
{
    class DebugRenderer
    {
    public:
        virtual ~DebugRenderer() = default;
        virtual void Draw() = 0;
    };
}
