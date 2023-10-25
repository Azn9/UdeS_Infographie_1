#pragma once
#include <extensions/PxFixedJoint.h>

#include "../../../../Public/Component/Component.h"
#include "../../../../Public/Util/Axis.h"

namespace PM3D_API
{
class Rigidbody : public Component
{
public:
	Rigidbody() = default;

	void Initialize() override;

	physx::PxRigidDynamic* GetActor() const { return actor; }

	void UpdateGlobalPose() const;
	void UpdateRenderPos() const;

	void SetTranslationLock(Axis axis);
	void SetRotationLock(Axis axis);

	void DrawDebugInfo() const override;
protected:
	physx::PxRigidDynamic* actor;
	physx::PxD6Joint* fixedJoint;
	
};
}
