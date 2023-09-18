#include "Api/Public/Component/Component.h"

PM3D_API::Component::Component(
    const Anchor anchor,
    const Anchor origin
): ContainerComponent(anchor, origin)
{
    children.emplace_back(this);
}

void PM3D_API::Component::Draw()
{
    
}
