#pragma once

#include "Collider.h"

namespace PM3D_API
{
    class PlaneCollider : public Collider
    {
    public:
        explicit PlaneCollider(
            physx::PxMaterial* _material
        ) : Collider(_material), shape(nullptr)
        {
        }

        void Initialize() override;

    protected:
        physx::PxShape* shape;
    };
}
