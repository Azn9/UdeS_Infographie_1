#pragma once

#include "Collider.h"
#include "Api/Public/Component/Basic/Render/3D/InstancedMeshRenderer.h"

namespace PM3D_API
{
    class InstancedMeshCollider final : public Component
    {
    public:
        explicit InstancedMeshCollider(
            physx::PxMaterial* _material,
            InstancedMeshRenderer* _meshRenderer
        ) : shape(nullptr), material(_material), meshRenderer(_meshRenderer)
        {
        }

        void Initialize() override;

        physx::PxShape* getShape() const { return shape; }

    protected:
        physx::PxShape* shape;
        physx::PxMaterial* material;

        InstancedMeshRenderer* meshRenderer;
    };
}
