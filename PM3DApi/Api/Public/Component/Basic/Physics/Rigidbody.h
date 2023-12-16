#pragma once
#include <extensions/PxFixedJoint.h>

#include "Api/Public/Component/Component.h"
#include "Api/Public/Util/Axis.h"
#include "../../../../../../libs/physx_include/PxPhysicsAPI.h"

namespace PM3D_API
{
    class Rigidbody : public Component
    {
    public:
        Rigidbody() : Rigidbody(false)
        {
        }

        explicit Rigidbody(const bool isStatic) : isStatic(isStatic), actor(nullptr), fixedJoint(nullptr)
        {
        }

        void Initialize() override;

        physx::PxRigidActor* GetActor() const { return actor; }

        void UpdateGlobalPose() const;
        void UpdateRenderPos() const;

        void SetTranslationLock(Axis axis);
        void SetRotationLock(Axis axis);

        void DrawDebugInfo() const override;

        bool IsStatic() const { return isStatic; }

        physx::PxRigidDynamic* getRigidDynamic()
        {
            if (isStatic)
                return nullptr;
            else
                return static_cast<physx::PxRigidDynamic*>(actor);
        }

    protected:
        bool isStatic;
        physx::PxRigidActor* actor;
        physx::PxD6Joint* fixedJoint;
    };
}
