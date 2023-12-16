#pragma once

#include "Collider.h"

namespace PM3D_API
{
    class BoxCollider : public Collider
    {
    public:
        explicit BoxCollider(
            physx::PxMaterial* _material
        ) : Collider(_material), shape(nullptr)
        {
        }

        void Initialize() override;

    protected:
        physx::PxShape* shape;
    };
}
