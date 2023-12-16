#pragma once

namespace PM3D_API
{
    /**
     * \brief Represents an event that can be sent to the EventSystem.
     */
    class Event
    {
    public:
        Event() = default;
        virtual ~Event() = default;
    };
}
