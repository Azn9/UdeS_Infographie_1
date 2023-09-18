#pragma once
#include <vector>

#include "Anchor.h"

namespace PM3D_API
{
    class Component;

    class ContainerComponent
    {
    public:
        ContainerComponent(Anchor anchor, Anchor origin);
        virtual ~ContainerComponent();

        Anchor GetAnchor() const { return anchor; }
        Anchor GetOrigin() const { return origin; }

        virtual void Draw();
        
    protected:
        std::vector<Component*> children;

    private:
        Anchor anchor;
        Anchor origin;
    };
}
