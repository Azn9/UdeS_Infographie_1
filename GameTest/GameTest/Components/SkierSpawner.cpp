#include "SkierSpawner.h"

#include "Api/Public/Scene/Scene.h"
#include "GameTest/Scenes/MainScene.h"
#include "GameTest/Objects/Right_Ski.h"
#include "Api/Public/Component/Basic/Physics/SkierCollider.h"
#include "JointsBreakerComponent.h"
#include "SkierDeleter.h"
#include "Api/Public/Util/FilterGroup.h"
#include "ToggleOnSkierComponent.h"
#include "GameTest/Objects/objWithChargeur.h"

#include "Api/Public/EventSystem/RespawnSkierEvent.h"


void SkierSpawner::Initialize()
{
    PM3D_API::EventSystem::Subscribe([this](const RespawnSkierEvent&)
    {
        _reSpawn = true;
        Update();
    });
    PM3D_API::EventSystem::Subscribe([this](const DeletedSkierEvent&)
    {
        _deletedSkierCount++;
        if (_deletedSkierCount == _current_id * 3)
            PM3D_API::EventSystem::Publish(RespawnSkierEvent());
    });

    auto shaderSkier = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    _meshrendererSkier = std::make_unique<PM3D_API::MeshRenderer>(std::move(shaderSkier), "skier.obj");
    _chargeurSkier = static_cast<PM3D::FastobjChargeur*>(_meshrendererSkier.get()->getChargeur());


    auto shaderlSki = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    _meshrendererlSki = std::make_unique<PM3D_API::MeshRenderer>(std::move(shaderlSki), "Left_ski.obj");
    _chargeurlSki = static_cast<PM3D::FastobjChargeur*>(_meshrendererlSki.get()->getChargeur());

    auto shaderrSki = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    _meshrendererrSki = std::make_unique<PM3D_API::MeshRenderer>(std::move(shaderrSki), "Right_ski.obj");
    _chargeurrSki = static_cast<PM3D::FastobjChargeur*>(_meshrendererrSki.get()->getChargeur());

    auto lsmrC = std::make_unique<SkiersMultiRenderer>("LeftSki", _chargeurlSki);
    leftSkiMultiRenderer = lsmrC.get();

    auto rsmrC = std::make_unique<SkiersMultiRenderer>("RightSki", _chargeurrSki);
    rightSkiMultiRenderer = rsmrC.get();

    auto smrC = std::make_unique<SkiersMultiRenderer>("Skier", _chargeurSkier);
    skierMultiRenderer = smrC.get();

    parentObject->AddChild(std::move(lsmrC));
    parentObject->AddChild(std::move(rsmrC));
    parentObject->AddChild(std::move(smrC));

    leftSkiMultiRenderer->Initialize();
    rightSkiMultiRenderer->Initialize();
    skierMultiRenderer->Initialize();
}

void SkierSpawner::Update()
{
    if (_reSpawn)
    {
        _scene = parentObject->GetScene();
        _current_id = 0;
        _deletedSkierCount = 0;
        SkierWave1();
        SkierWave2();
        SkierWave3();
        SkierWave4();
        SkierWave5();
        _reSpawn = false;
        PM3D::Time::GetInstance().SetTimeScale(1.f);
    }
}

void SkierSpawner::DrawSelf() const
{
    //    skierMultiRenderer->DrawSelf();
    //    leftSkiMultiRenderer->DrawSelf();
    //    rightSkiMultiRenderer->DrawSelf();
}

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
    // right ski
    auto rski = std::make_unique<PM3D_API::GameObject>("rski");
    const auto rskiPtr = rski.get();
    _scene->AddChild(std::move(rski));
    rskiPtr->SetWorldScale(XMFLOAT3(10.f, 10.f, 10.f));
    rskiPtr->SetWorldPosition(position);
    rskiPtr->Initialize();
    rskiPtr->AddComponent(std::move(std::make_unique<ToggleOnSkierComponent>(checkpoint)));
    rskiPtr->AddComponent(std::move(std::make_unique<SkierDeleter>()));

    rightSkiMultiRenderer->Add(rskiPtr);

    auto rskiRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto rskiRigidbodyPtr = rskiRigidbody.get();
    rskiPtr->AddComponent(std::move(rskiRigidbody));
    rskiRigidbodyPtr->Initialize();

    auto rSkiRigidDynamic = rskiRigidbodyPtr->getRigidDynamic();
    rSkiRigidDynamic->setRigidDynamicLockFlags(
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z /*| physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X*/
        /*| physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y*/);
    rSkiRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);


    // left ski
    auto lski = std::make_unique<PM3D_API::GameObject>("lski");
    const auto lskiPtr = lski.get();
    _scene->AddChild(std::move(lski));
    lskiPtr->SetWorldScale(XMFLOAT3(10.f, 10.f, 10.f));
    lskiPtr->SetWorldPosition(position);
    lskiPtr->Initialize();

    leftSkiMultiRenderer->Add(lskiPtr);

    auto lskiRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto lskiRigidbodyPtr = lskiRigidbody.get();
    lskiPtr->AddComponent(std::move(lskiRigidbody));
    lskiRigidbodyPtr->Initialize();

    auto lSkiRigidDynamic = rskiRigidbodyPtr->getRigidDynamic();
    lSkiRigidDynamic->setRigidDynamicLockFlags(
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z /* | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X*/
        /*| physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y*/);
    lSkiRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

    auto lskiCollider = std::make_unique<PM3D_API::SkierCollider>(PxGetPhysics().createMaterial(0.f, 0.f, 1.f));
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
    auto skier = std::make_unique<PM3D_API::GameObject>("lski");
    const auto skierPtr = skier.get();
    _scene->AddChild(std::move(skier));
    skierPtr->SetWorldScale(XMFLOAT3(10.f, 10.f, 10.f));
    skierPtr->SetWorldPosition(position);
    skierPtr->Initialize();
    skierPtr->AddComponent(std::move(std::make_unique<ToggleOnSkierComponent>(checkpoint)));
    skierPtr->AddComponent(std::move(std::make_unique<SkierDeleter>()));

    skierMultiRenderer->Add(skierPtr);

    auto skierRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto skierRigidbodyPtr = skierRigidbody.get();
    skierPtr->AddComponent(std::move(skierRigidbody));
    skierRigidbodyPtr->Initialize();

    auto skierRigidDynamic = rskiRigidbodyPtr->getRigidDynamic();
    skierRigidDynamic->setRigidDynamicLockFlags(
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
    skierRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

    physx::PxShape* SkierShape;
    skierRigidbodyPtr->GetActor()->getShapes(&SkierShape, 1, 0);
    skierRigidbodyPtr->GetActor()->detachShape(*SkierShape);

    physx::PxShape* rSkiShape;
    rskiRigidbodyPtr->GetActor()->getShapes(&rSkiShape, 1, 0);
    rskiRigidbodyPtr->GetActor()->detachShape(*rSkiShape);

    //joints between skis and skier
    auto jointSki = physx::PxFixedJointCreate(*_scene->GetPhysicsResolver()->GetPhysics(),
                                              lskiRigidbodyPtr->GetActor(), physx::PxTransform(physx::PxVec3(0, 0, 0),
                                                  physx::PxQuat(0, 0, 0, 1)),
                                              rskiRigidbodyPtr->GetActor(), physx::PxTransform(physx::PxVec3(0, 0, 0),
                                                  physx::PxQuat(0, 0, 0, 1)));


    auto jointSkier = physx::PxD6JointCreate(*_scene->GetPhysicsResolver()->GetPhysics(),
                                             skierRigidbodyPtr->GetActor(), physx::PxTransform(
                                                 physx::PxVec3(0.f, -0.2f, 0.f),
                                                 physx::PxQuat(0, 0, 0, 1)),
                                             lskiRigidbodyPtr->GetActor(), physx::PxTransform(
                                                 physx::PxVec3(0.f, 0.f, 0.f),
                                                 physx::PxQuat(0, 0, 0, 1)));


    jointSkier->setMotion(physx::PxD6Axis::eSWING1, physx::PxD6Motion::eLIMITED);
    jointSkier->setPyramidSwingLimit(physx::PxJointLimitPyramid(-0.3f, 0.3f, 0.f, 0.f));

    jointSkier->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);
    jointSkier->setDistanceLimit(physx::PxJointLinearLimit(0.2f));
}

void SkierSpawner::SkierWave1()
{
    for (int i = 0; i != 5; ++i)
    {
        AddSkier(DirectX::XMFLOAT3(-20.f + i * 5.f, -90.f, -60.f), _current_id, 1);
        _current_id++;
    }
}

void SkierSpawner::SkierWave2()
{
    for (int i = 0; i != 5; ++i)
    {
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
