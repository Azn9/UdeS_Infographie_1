#pragma once

#include "Collider.h"

namespace PM3D_API
{
class SphereCollider : public Collider
{
public:
	explicit SphereCollider(
		physx::PxMaterial* _material
	) : Collider(_material), shape(nullptr) {}

	void Initialize() override;

	physx::PxShape* getShape() { return shape; };

protected:
	physx::PxShape* shape;
};
}
