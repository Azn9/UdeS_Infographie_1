#pragma once
#include <PxMaterial.h>

#include "Api/Public/Component/Component.h"

namespace PM3D_API
{
class Collider : public Component
{
public:
	explicit Collider(physx::PxMaterial* material) : material(material)
	{
	}

	void Initialize() override;

	physx::PxMaterial* GetMaterial() const { return material; }

protected:
	physx::PxMaterial* material;
};
}
