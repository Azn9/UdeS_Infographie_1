#pragma once

#include "Collider.h"

namespace PM3D_API
{
    class InstancedMeshCollider final : public Component
    {
    public:
        explicit InstancedMeshCollider(
            physx::PxMaterial* _material
        ) : shape(nullptr), material(_material)
        {
        }

        void Initialize() override;

        physx::PxShape* getShape() const { return shape; }

    protected:
        physx::PxShape* shape;
        physx::PxMaterial* material;
    };
}
