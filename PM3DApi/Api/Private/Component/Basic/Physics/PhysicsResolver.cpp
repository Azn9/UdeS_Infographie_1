#include "../../../../Public/Component/Basic/Physics/PhysicsResolver.h"
#include "../../../../Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "../../../../Public/GameHost.h"
#include "Api/Public/Util/FilterShader.h"
#include "Api/Public/Util/SimulationCallback.h"

void PM3D_API::PhysicsResolver::Initialize()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

	if (physx::PxPvdTransport* mTransport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10000); mTransport !=
		nullptr)
	{
		const physx::PxPvdInstrumentationFlags mPvdFlags = physx::PxPvdInstrumentationFlag::eALL;

		pvd = physx::PxCreatePvd(*foundation);
		pvd->connect(*mTransport, mPvdFlags);
	}

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pvd);

	InitializeScene();
}

void PM3D_API::PhysicsResolver::InitializeScene()
{
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = FilterShader;

	SimulationCallback* simulationCallback = new SimulationCallback;
	sceneDesc.simulationEventCallback = simulationCallback;

	scene = physics->createScene(sceneDesc);

	scene->setContactModifyCallback(simulationCallback);

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
}

void PM3D_API::PhysicsResolver::ResolvePhysics()
{
	const auto physicsDeltaTime = PM3D::Time::GetInstance().GetPhysicsDeltaTime();
	if (physicsDeltaTime == 0)
	{
		return;
	}

	std::cout << "Physics update" << std::endl;

	scene->simulate(physicsDeltaTime);
	scene->fetchResults(true);

	for (const auto& gameObject : parentObject->GetScene()->GetChildren())
	{
		if (const auto rigidbody = gameObject->GetComponent<Rigidbody>())
		{
			const auto actor = rigidbody->GetActor();

			if (rigidbody->IsStatic())
			{
				continue; // Il n'a pas pu bouger
			}
			
			if (const physx::PxRigidDynamic* dynamic = static_cast<physx::PxRigidDynamic*>(actor); dynamic->isSleeping())
				continue;

			rigidbody->UpdateRenderPos();
		}
	}
}
