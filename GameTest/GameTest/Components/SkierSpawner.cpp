#include "SkierSpawner.h"

#include <Api/Public/Scene/Scene.h>
#include <GameTest/Scenes/MainScene.h>
#include <GameTest/Objects/Right_Ski.h>
#include <GameTest/Objects/Left_ski.h>
#include <Api/Public/Component/Basic/Physics/SkierCollider.h>
#include "JointsBreakerComponent.h"
#include "SkierDeleter.h"
#include <GameTest/Objects/Skier.h>
#include "Api/Public/Util/FilterGroup.h"
#include "Api/Public/GameHost.h"
#include <Core/Public/Mesh/CSommetMesh.h>
#include "ToggleOnSkierComponent.h"



/// <summary>
/// Add a skier
/// </summary>
/// <param name="position"></param>
/// <param name="id"></param>
/// <param name="phyicsResolver"></param>
/// <param name="velocity"></param>
/// <param name="acceleration"></param>
void SkierSpawner::AddSkier(
	const DirectX::XMFLOAT3& position,
	physx::PxU32 id,
	int checkpoint
)
{
	auto scene = parentObject->GetScene();

	// right ski
	auto rski = std::make_unique<Right_Ski>();
	const auto rskiPtr = rski.get();
	scene->AddChild(std::move(rski));
	rskiPtr->SetWorldScale(XMFLOAT3(3.f, 3.f, 3.f));
	rskiPtr->SetWorldPosition(position);
	rskiPtr->Initialize();
	rskiPtr->AddComponent(std::move(std::make_unique<ToggleOnSkierComponent>(checkpoint)));
	rskiPtr->AddComponent(std::move(std::make_unique<SkierDeleter>()));

	auto rskiRigidbody = std::make_unique<PM3D_API::Rigidbody>();
	const auto rskiRigidbodyPtr = rskiRigidbody.get();
	rskiPtr->AddComponent(std::move(rskiRigidbody));
	rskiRigidbodyPtr->Initialize();
	rskiRigidbodyPtr->getRigidDynamic()->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z /*| physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X*/ /*| physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y*/);

	rskiRigidbodyPtr->getRigidDynamic()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);


	// left ski
	auto lski = std::make_unique<Left_Ski>();
	const auto lskiPtr = lski.get();
	scene->AddChild(std::move(lski));
	lskiPtr->SetWorldScale(XMFLOAT3(3.f, 3.f, 3.f));
	lskiPtr->SetWorldPosition(position);
	lskiPtr->Initialize();

	auto lskiRigidbody = std::make_unique<PM3D_API::Rigidbody>();
	const auto lskiRigidbodyPtr = lskiRigidbody.get();
	lskiPtr->AddComponent(std::move(lskiRigidbody));
	lskiRigidbodyPtr->Initialize();
	lskiRigidbodyPtr->getRigidDynamic()->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z/* | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X*/ /*| physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y*/);

	lskiRigidbodyPtr->getRigidDynamic()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

	auto lskiCollider = std::make_unique<
		PM3D_API::SkierCollider>(PxGetPhysics().createMaterial(0.f, 0.f, 1.f));
	const auto lskiColliderPtr = lskiCollider.get();
	lskiPtr->AddComponent(std::move(lskiCollider));
	lskiPtr->AddComponent(std::move(std::make_unique<JointsBreakerComponent>()));
	lskiPtr->AddComponent(std::move(std::make_unique<ToggleOnSkierComponent>(checkpoint)));
	lskiPtr->AddComponent(std::move(std::make_unique<SkierDeleter>()));

	lskiColliderPtr->Initialize();
	physx::PxFilterData filterDataSkier;
	filterDataSkier.word0 = FilterGroup::eSKIER;
	filterDataSkier.word1 = physx::PxU32(id);
	physx::PxShape* lskiShape = lskiColliderPtr->getShape();
	lskiShape->setSimulationFilterData(filterDataSkier);



	// skier
	auto skier = std::make_unique<Skier>();
	const auto skierPtr = skier.get();
	scene->AddChild(std::move(skier));
	skierPtr->SetWorldScale(XMFLOAT3(3.f, 3.f, 3.f));
	skierPtr->SetWorldPosition(position);
	skierPtr->Initialize();
	skierPtr->AddComponent(std::move(std::make_unique<ToggleOnSkierComponent>(checkpoint)));
	skierPtr->AddComponent(std::move(std::make_unique<SkierDeleter>()));
	auto skierRigidbody = std::make_unique<PM3D_API::Rigidbody>();
	const auto skierRigidbodyPtr = skierRigidbody.get();
	skierPtr->AddComponent(std::move(skierRigidbody));
	skierRigidbodyPtr->Initialize();
	skierRigidbodyPtr->getRigidDynamic()->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);

	skierRigidbodyPtr->getRigidDynamic()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

	physx::PxShape* SkierShape;
	skierRigidbodyPtr->GetActor()->getShapes(&SkierShape, 1, 0);
	skierRigidbodyPtr->GetActor()->detachShape(*SkierShape);

	physx::PxShape* rSkiShape;
	rskiRigidbodyPtr->GetActor()->getShapes(&rSkiShape, 1, 0);
	rskiRigidbodyPtr->GetActor()->detachShape(*rSkiShape);

	//joints between skis and skier
	auto jointSki = physx::PxFixedJointCreate(*scene->GetPhysicsResolver()->GetPhysics(),
		lskiRigidbodyPtr->GetActor(), physx::PxTransform(physx::PxVec3(0, 0, 0),
			physx::PxQuat(0, 0, 0, 1)),
		rskiRigidbodyPtr->GetActor(), physx::PxTransform(physx::PxVec3(0, 0, 0),
			physx::PxQuat(0, 0, 0, 1)));


	auto jointSkier = physx::PxD6JointCreate(*scene->GetPhysicsResolver()->GetPhysics(),
		skierRigidbodyPtr->GetActor(), physx::PxTransform(physx::PxVec3(0.f, -0.2f, 0.f),
			physx::PxQuat(0, 0, 0, 1)),
		lskiRigidbodyPtr->GetActor(), physx::PxTransform(physx::PxVec3(0.f, 0.f, 0.f),
			physx::PxQuat(0, 0, 0, 1)));


	jointSkier->setMotion(physx::PxD6Axis::eSWING1, physx::PxD6Motion::eLIMITED);
	jointSkier->setPyramidSwingLimit(physx::PxJointLimitPyramid(-0.3f, 0.3f, 0.f, 0.f));

	jointSkier->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);
	jointSkier->setDistanceLimit(physx::PxJointLinearLimit(0.2f));
}

void SkierSpawner::SkierWave1()
{
	for (int i = 0; i != 5; ++i) {
		AddSkier(DirectX::XMFLOAT3(-20.f + i * 5.f, -90.f, -60.f), _current_id, 1);
		_current_id++;
	}
}
void SkierSpawner::SkierWave2()
{
	for (int i = 0; i != 5; ++i) {
		AddSkier(DirectX::XMFLOAT3(-20.f + i * 5.f, -230.f, -300.f), _current_id, 2);
		_current_id++;
	}
}
void SkierSpawner::SkierWave3()
{
}
void SkierSpawner::SkierWave4()
{
}
void SkierSpawner::SkierWave5()
{
}

