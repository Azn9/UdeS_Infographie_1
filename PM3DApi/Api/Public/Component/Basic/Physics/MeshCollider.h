#pragma once

#include "Collider.h"

namespace PM3D_API
{
	class MeshCollider : public Collider
	{
	public:
		explicit MeshCollider(
			physx::PxMaterial* _material
		) : Collider(_material), shape(nullptr) {}

		void Initialize() override;

	protected:
		physx::PxShape* shape;
	};
}
