#include "Api/Public/Component/Default/DrawSizePreservingFillComponent.h"

PM3D_API::DrawSizePreservingFillComponent::DrawSizePreservingFillComponent(
    const Vector2 size,
    const DrawSizePreservationStrategy preservationStrategy
): ContainerComponent(MIDDLE_CENTER, MIDDLE_CENTER),
    size(size),
    preservationStrategy(preservationStrategy)
{
}

PM3D_API::DrawSizePreservingFillComponent::~DrawSizePreservingFillComponent()
{
    
}

void PM3D_API::DrawSizePreservingFillComponent::Draw()
{
    // TODO

    
}
