#pragma once

#include "Collider.h"

namespace PM3D_API
{
    class SkierCollider : public Collider
    {
    public:
        explicit SkierCollider(
            physx::PxMaterial* _material
        ) : Collider(_material), shape(nullptr)
        {
        }

        void Initialize() override;

        physx::PxShape* getShape() { return shape; };

    protected:
        physx::PxShape* shape;
    };
}
