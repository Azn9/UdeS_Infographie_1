#pragma once
#include <cstdint>

#include "DebugRenderer.h"

namespace PM3D_API
{
    class FPSDebugRenderer final : public DebugRenderer
    {
    public:
        FPSDebugRenderer() : lastUpdate(0)
        {
        };
        ~FPSDebugRenderer() override = default;

        void Draw() override;

    private:
        int64_t lastUpdate;
    };
}
