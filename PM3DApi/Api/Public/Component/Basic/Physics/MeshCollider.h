#pragma once

#include "Collider.h"
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"

namespace PM3D_API
{
	class MeshCollider : public Collider
	{
	public:
		explicit MeshCollider(
			physx::PxMaterial* _material,
			MeshRenderer* _meshRenderer
		) : Collider(_material), shape(nullptr), meshRenderer(_meshRenderer) {}

		void Initialize() override;

		physx::PxShape* getShape() { return shape; }

	protected:
		physx::PxShape* shape;
		MeshRenderer* meshRenderer;
	};
}
