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
#include "Api/Public/GameObject/Basic/BasicSphere.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Components/MainScene/PauseComponent.h"
#include "GameTest/Objects/Pine.h"

#include "GameTest/Components/MovableComponent.h"
#include "GameTest/Components/SizeModifierComponent.h"
#include "GameTest/Components/WalkSoundComponent.h"
#include "GameTest/UI/GameUI.h"
#include "GameTest/Objects/MainScene/Map.h"

#include "Api/Private/Light/Shadow/ShadowProcessor.h"

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
        PM3D_API::Camera::PERSPECTIVE,
        XMFLOAT3(-2.0f, -50.0f, 50.0f),
        XMVectorSet(-2.0f, -50.0f, 80.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
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
    // ============= Base elements from the loading scene =====
    auto objects = std::make_unique<GameObject>("Objects");
    objects->Initialize();
    const auto objectsPtr = objects.get();
    auto objectsShader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    objects->AddComponent(
        std::make_unique<PM3D_API::MeshRenderer>(std::move(objectsShader), "LoadingScene/objects.obj"));
    AddChild(std::move(objects));

    auto objectsRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto objectsRigidbodyPtr = objectsRigidbody.get();
    objectsPtr->AddComponent(std::move(objectsRigidbody));
    objectsRigidbodyPtr->Initialize();

    auto objectsMeshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto objectsMeshColliderPtr = objectsMeshCollider.get();
    objectsPtr->AddComponent(std::move(objectsMeshCollider));
    objectsMeshColliderPtr->Initialize();

    // ============= Add the map =============

    auto map = std::make_unique<Map>();
    const auto mapPtr = map.get();
    AddChild(std::move(map));


    mapPtr->Initialize();


    // ============= Add a sphere =============
    {
        auto sphere = std::make_unique<PM3D_API::BasicSphere>("Sphere");
        spherePtr = sphere.get();
        AddChild(std::move(sphere));
        spherePtr->SetWorldScale(XMFLOAT3(.2f, .2f, .2f));
        spherePtr->SetWorldPosition(XMFLOAT3(6.f, -50.f, 66.f));
        spherePtr->Initialize();

        auto sphereRigidbody = std::make_unique<PM3D_API::Rigidbody>();
        const auto sphereRigidbodyPtr = sphereRigidbody.get();
        spherePtr->AddComponent(std::move(sphereRigidbody));
        sphereRigidbodyPtr->Initialize();

        auto sphereCollider = std::make_unique<
            PM3D_API::SphereCollider>(PxGetPhysics().createMaterial(0.4f, 0.4f, 0.f));
        const auto sphereColliderPtr = sphereCollider.get();
        spherePtr->AddComponent(std::move(sphereCollider));
        sphereColliderPtr->Initialize();
        physx::PxFilterData filterDataSnowball;
        filterDataSnowball.word0 = FilterGroup::eSNOWBALL;
        physx::PxShape* sphereShape = sphereColliderPtr->getShape();
        sphereShape->setSimulationFilterData(filterDataSnowball);

        GetMainCamera()->GetComponent<CameraFollowComponent>()->SetObjectToFollow(spherePtr);

        spherePtr->AddComponent(std::make_unique<SizeModifierComponent>());

        spherePtr->AddComponent(std::make_unique<MovableComponent>());

        auto walkSoundComponent = std::make_unique<WalkSoundComponent>();
        const auto walkSoundComponentPtr = walkSoundComponent.get();
        spherePtr->AddComponent(std::move(walkSoundComponent));
        walkSoundComponentPtr->Initialize();
    }
    
    auto shadowProcessor = std::make_unique<ShadowProcessor>();
    shadowProcessor->Initialize();
    shadowProcessor->SetScene(this);
    AddComponent(std::move(shadowProcessor));
}

void MainScene::InitializeUI()
{
    Scene::InitializeUI(); // Init the base canvas

    auto gameUI = std::make_unique<GameUI>();
    const auto gameUIPtr = gameUI.get();
    AddUiChild(std::move(gameUI));
    gameUIPtr->Initialize();
}
