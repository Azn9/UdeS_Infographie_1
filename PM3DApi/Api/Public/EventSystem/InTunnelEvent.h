#pragma once
#include "Api/Public/EventSystem/Event.h"

class InTunnelEvent final : public PM3D_API::Event
{
public:
    explicit InTunnelEvent(const bool inTunnel) : _inTunnel(inTunnel)
    {
    }

    [[nodiscard]] bool IsInTunnel() const { return _inTunnel; }

private:
    bool _inTunnel;
};
