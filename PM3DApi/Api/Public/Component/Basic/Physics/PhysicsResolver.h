#pragma once

#include <set>
#include "../../Component.h"
#include "../../../../../../libs/physx_include/PxPhysicsAPI.h"

#ifndef PHYSX_SNIPPET_PVD_H
#define PHYSX_SNIPPET_PVD_H
#define PVD_HOST "127.0.0.1"
#endif

namespace PM3D_API
{
class PhysicsResolver : public Component
{
public:
	PhysicsResolver() = default;
	~PhysicsResolver() override = default;
	
	void Initialize() override;
	virtual void InitializeScene();
	virtual void ResolvePhysics();

	void AddActor(physx::PxActor* actor) { actors.insert(actor); }
	void RemoveActor(physx::PxActor* actor) { actors.erase(actor); }

	physx::PxPhysics* GetPhysics() const { return physics; }
	physx::PxScene* GetScene() const { return scene; }
	physx::PxMaterial* GetDefaultMaterial() const { return defaultMaterial; }

protected:
	physx::PxDefaultAllocator allocator;
	physx::PxDefaultErrorCallback errorCallback;
	physx::PxFoundation* foundation;
	physx::PxPvd* pvd;
	physx::PxPhysics* physics;
	physx::PxDefaultCpuDispatcher* dispatcher;
	physx::PxScene* scene;
	physx::PxMaterial* defaultMaterial;

	std::set<physx::PxActor*> actors;
};
}
