#include "PhysicsResolver.h"

#include <iostream>
#include <ostream>

#include "Rigidbody.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "Api/Public/GameHost.h"

void PM3D_API::PhysicsResolver::Initialize()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

	pvd = physx::PxCreatePvd(*foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pvd);

	InitializeScene();
}

void PM3D_API::PhysicsResolver::InitializeScene()
{
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	scene = physics->createScene(sceneDesc);

	if (!scene)
	{
		throw std::runtime_error("PhysicsResolver::InitializeScene() - Failed to create scene");
	}

	if (physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient())
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// Create default material
	defaultMaterial = physics->createMaterial(0.4f, 0.4f, 0.0f);

	std::cout << "PhysicsResolver::InitializeScene()" << std::endl;
}

void PM3D_API::PhysicsResolver::ResolvePhysics()
{
	const auto physicsDeltaTime = PM3D::Time::GetInstance().GetPhysicsDeltaTime();
	if (physicsDeltaTime == 0)
	{
		return;
	}
	
	scene->simulate(physicsDeltaTime);
	scene->fetchResults(true);

	for (const auto & gameObject : GameHost::GetInstance()->GetScene()->GetChildren())
	{
		if (const auto rigidbody = gameObject->GetComponent<Rigidbody>())
			rigidbody->UpdateRenderPos();
	} 
}
