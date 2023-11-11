#include "MainScene.h"

#include <memory>

#include "Api/Public/Camera/Camera.h"

#include "Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Component/Basic/Physics/SphereCollider.h"
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Light/AmbiantLight.h"
#include "Api/Public/Util/FilterGroup.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/GameObject/Basic/BasicSphere.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Components/MainScene/PauseComponent.h"
#include "GameTest/UI/TestUIObject.h"
#include "GameTest/Objects/Pine.h"

#include "GameTest/Heightmap.h"
#include "GameTest/TimeScaleTest.h"
#include "GameTest/Components/MovableComponent.h"
#include "GameTest/Components/SizeModifierComponent.h"

void MainScene::InitializePhysics()
{
    auto physicsResolver = std::make_unique<PM3D_API::PhysicsResolver>();
    physicsResolver->Initialize();
    SetPhysicsResolver(std::move(physicsResolver));
}

void MainScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSECTIVE,
        XMFLOAT3(0.0f, 0.1f, 0.0f),
        XMVectorSet(0.0f, -5.0f, 15.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
    mainCamera->AddComponent(std::make_unique<CameraFollowComponent>());
    mainCamera->SetClearColor(XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
    SetMainCamera(std::move(mainCamera));
}

void MainScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        XMFLOAT3(1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void MainScene::InitializeObjects()
{
    // ============= Add a plane =============
    {
    auto map = std::make_unique<Heightmap>();
    map->SetWorldPosition(XMFLOAT3(0.0f, -10.0f, 10.0f));
    map->SetWorldScale(XMFLOAT3(10.f, 10.f, 10.f));
    map->Initialize();
    const auto mapPtr = map.get();
    AddChild(std::move(map));

    auto mapRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto mapRigidbodyPtr = mapRigidbody.get();
    mapPtr->AddComponent(std::move(mapRigidbody));
    mapRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto meshColliderPtr = meshCollider.get();
    mapPtr->AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();
    }

    // ============= Add a sphere =============
    {
    auto sphere = new PM3D_API::BasicSphere("Sphere");
    AddChild(std::unique_ptr<PM3D_API::BasicSphere>(sphere));
    sphere->SetWorldPosition(XMFLOAT3(0.0f, -9.5f, 10.0f));
    sphere->SetWorldScale(XMFLOAT3(.2f, .2f, .2f));
    sphere->Initialize();

    auto sphereRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto sphereRigidbodyPtr = sphereRigidbody.get();
    sphere->AddComponent(std::move(sphereRigidbody));
    sphereRigidbodyPtr->Initialize();

    auto sphereCollider = std::make_unique<PM3D_API::SphereCollider>(PxGetPhysics().createMaterial(0.4f, 0.4f, 0.f));
    const auto sphereColliderPtr = sphereCollider.get();
    sphere->AddComponent(std::move(sphereCollider));
    sphereColliderPtr->Initialize();
    physx::PxFilterData filterDataSnowball;
    filterDataSnowball.word0 = FilterGroup::eSNOWBALL;
    physx::PxShape* sphereShape = sphereColliderPtr->getShape();
    sphereShape->setSimulationFilterData(filterDataSnowball);

    GetMainCamera()->GetComponent<CameraFollowComponent>()->SetObjectToFollow(sphere);

    sphere->AddComponent(std::make_unique<SizeModifierComponent>());

    sphere->AddComponent(std::make_unique<MovableComponent>());
    }
    

    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));
    PM3D::Time::GetInstance().SetTimeScale(0.0f);
    
    // ============= Add railings =============
    {
    auto railings = std::make_unique<GameObject>("railings");
    auto shader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    railings->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), "Railing.obj"));
    railings->Initialize();
    const auto railingsPtr = railings.get();
    AddChild(std::move(railings));

    auto railingsRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto railingsRigidbodyPtr = railingsRigidbody.get();
    railingsPtr->AddComponent(std::move(railingsRigidbody));
    railingsRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto meshColliderPtr = meshCollider.get();
    railingsPtr->AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();

    AddChild(std::move(railings));
    }
    
    // ============= Add a pine =============
    auto pine = std::make_unique<Pine>();
    pine->SetWorldPosition(XMFLOAT3(-0.4f, -12.0f, 14.0f));
    pine->SetWorldScale(XMFLOAT3(1.5f, 1.5f, 1.5f));
    pine->SetWorldRotation(XMFLOAT3(0.0f, 0.90f, 0.f));
    pine->Initialize();
    const auto pinePtr = pine.get();
    AddChild(std::move(pine));

    auto pineRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto pineRigidbodyPtr = pineRigidbody.get(); 
    pinePtr->AddComponent(std::move(pineRigidbody));
    pineRigidbodyPtr->Initialize();

    auto pineMeshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto pineMeshColliderPtr = pineMeshCollider.get();
    pinePtr->AddComponent(std::move(pineMeshCollider));
    pineMeshColliderPtr->Initialize();
    physx::PxFilterData filterDataObstacle;
    filterDataObstacle.word0 = FilterGroup::eOBSTACLE;
    physx::PxShape* treeShape = pineMeshColliderPtr->getShape();
    treeShape->setSimulationFilterData(filterDataObstacle);
}

void MainScene::InitializeUI()
{
    Scene::InitializeUI(); // Init the base canvas

    auto pauseComponent = std::make_unique<PauseComponent>();
    const auto pauseComponentPtr = pauseComponent.get();
    AddUiChild(std::move(pauseComponent));
    pauseComponentPtr->Initialize();
}
