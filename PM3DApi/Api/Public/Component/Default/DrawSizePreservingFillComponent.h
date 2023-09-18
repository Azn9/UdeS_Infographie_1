#pragma once
#include "DrawSizePreservationStrategy.h"
#include "Api/Public/Component/ContainerComponent.h"
#include "Api/Public/Util/Vector2.h"

namespace PM3D_API
{
    class DrawSizePreservingFillComponent : public ContainerComponent
    {
    public:
        explicit DrawSizePreservingFillComponent(Vector2 size, DrawSizePreservationStrategy preservationStrategy);

        Vector2 GetSize() const { return size; }
        DrawSizePreservationStrategy GetPreservationStrategy() const { return preservationStrategy; }

        void Draw() override;
    protected:
        ~DrawSizePreservingFillComponent() override;

    private:
        Vector2 size;
        DrawSizePreservationStrategy preservationStrategy;
    };
}
