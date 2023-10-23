#pragma once

#include "../../Component.h"

namespace PM3D_API
{
class PhysicsResolver : public Component
{
public:
	PhysicsResolver() = default;
	~PhysicsResolver() override = default;
	
	void Initialize() override;
	virtual void ResolvePhysics();

protected:

private:
	
};
}
