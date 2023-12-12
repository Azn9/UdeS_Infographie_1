#pragma once
#include "Event.h"

struct CollisionSkierEvent : public PM3D_API::Event
{
	CollisionSkierEvent(const physx::PxU32 id) : Event(), _id(id) {}

	[[nodiscard]] physx::PxU32 GetId() const { return _id; }

private:
	physx::PxU32 _id;

};