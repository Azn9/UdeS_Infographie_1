#pragma once
#include "Anchor.h"
#include "ContainerComponent.h"

namespace PM3D_API
{
    class Component : public ContainerComponent
    {
    public:
        Component(Anchor anchor, Anchor origin);
        ~Component() override = default;
        
        void Draw() override;

    protected:
        
    };
}
