#pragma once

class DebugRenderer
{
public:
    virtual ~DebugRenderer() = default;
    virtual void Draw() = 0;
};
