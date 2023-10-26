#pragma once
#include <extensions/PxFixedJoint.h>

#include "../../../../Public/Component/Component.h"
#include "../../../../Public/Util/Axis.h"

namespace PM3D_API
{
class Rigidbody : public Component
{
public:
	Rigidbody() : Rigidbody(false) {}
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
protected:
	bool isStatic;
	physx::PxRigidActor* actor;
	physx::PxD6Joint* fixedJoint;
	
};
}
