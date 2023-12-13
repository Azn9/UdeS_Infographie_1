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
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Components/MainScene/PauseComponent.h"
#include "GameTest/Objects/Pine.h"

#include "GameTest/Heightmap.h"
#include "GameTest/Tunnel.h"
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
        PM3D_API::Camera::PERSPECTIVE,
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
    // ============= Add the map =============
    {
        auto map = std::make_unique<Heightmap>();
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

    // ============= Add the tunnel =============
    {
        auto tunnel = std::make_unique<Tunnel>();
        tunnel->Initialize();
        const auto tunnelPtr = tunnel.get();
        AddChild(std::move(tunnel));

        auto tunnelRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
        const auto tunnelRigidbodyPtr = tunnelRigidbody.get();
        tunnelPtr->AddComponent(std::move(tunnelRigidbody));
        tunnelRigidbodyPtr->Initialize();

        auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
        const auto meshColliderPtr = meshCollider.get();
        tunnelPtr->AddComponent(std::move(meshCollider));
        meshColliderPtr->Initialize();

        physx::PxFilterData filterDataTunnel;
        filterDataTunnel.word0 = FilterGroup::eTUNNEL;
        physx::PxShape* tunnelShape = meshColliderPtr->getShape();
        tunnelShape->setSimulationFilterData(filterDataTunnel);
    }

    // ============= Add a sphere =============
    {
        auto sphere = std::make_unique<PM3D_API::BasicSphere>("Sphere");
        const auto spherePtr = sphere.get();
        AddChild(std::move(sphere));
        spherePtr->SetWorldScale(XMFLOAT3(.2f, .2f, .2f));
        spherePtr->SetWorldPosition(XMFLOAT3(0.f, -60.f, 0.f));
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

        auto cameraRealFP = std::make_unique<PM3D_API::Camera>(
            "Camera RFP",
            PM3D_API::Camera::PERSPECTIVE,
            XMFLOAT3(0.0f, 0.1f, 0.0f),
            XMVectorSet(0.0f, -5.0f, 15.0f, 1.0f)
        );
        cameraRealFP->SetFieldOfView(45.0f);
        cameraRealFP->SetFarDist(1000.0f);
        cameraRealFP->SetClearColor(XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));

        spherePtr->AddChild(std::move(cameraRealFP));
    }

    // ============= Add railings =============
    {
        auto railings = std::make_unique<GameObject>("railings");
        auto shader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
        railings->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), "Railings.obj"));
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

        physx::PxFilterData filterDataObstacle;
        filterDataObstacle.word0 = FilterGroup::eOBSTACLE;
        physx::PxShape* railingShape = meshColliderPtr->getShape();
        railingShape->setSimulationFilterData(filterDataObstacle);
    }

    // ============= Add a pines =============
    {
        AddPine(XMFLOAT3(-0.05f, -10.56f, -14.89f));
        AddPine(XMFLOAT3(1.2f, 14.f, -21.f));
        AddPine(XMFLOAT3(-2.1f, -12.6f, -18.f));
        AddPine(XMFLOAT3(-3.75f, -14.54f, -21.94f));
        AddPine(XMFLOAT3(1.2f, -14.1f, -21.f));
        AddPine(XMFLOAT3(12.87f, -27.54f, -44.46f));
        AddPine(XMFLOAT3(16.96f, -35.3f, -57.02f));
        AddPine(XMFLOAT3(14.53f, -31.53f, -51.1f));
        AddPine(XMFLOAT3(-15.4f, -35.8f, -60.16f));
        AddPine(XMFLOAT3(-27.11f, -35.67f, -57.92f));
        AddPine(XMFLOAT3(-22.06f, -21.8f, -35.15f));
        AddPine(XMFLOAT3(10.83f, -43.02f, -71.44f));
        AddPine(XMFLOAT3(22.33f, -20.25f, -29.9f));
        AddPine(XMFLOAT3(-12.64f, -36.24f, -61.14f));
        AddPine(XMFLOAT3(-10.16f, -35.38f, -59.47f));
        AddPine(XMFLOAT3(-21.72f, -40.49f, -68.81f));
        AddPine(XMFLOAT3(2.31f, -32.77f, -54.46f));
        AddPine(XMFLOAT3(5.0f, -33.96f, -56.48f));
        AddPine(XMFLOAT3(21.97f, -34.6f, -55.14f));
    }
}


void MainScene::InitializeUI()
{
    Scene::InitializeUI(); // Init the base canvas

    auto pauseComponent = std::make_unique<PauseComponent>();
    const auto pauseComponentPtr = pauseComponent.get();
    AddUiChild(std::move(pauseComponent));
    pauseComponentPtr->Initialize();
}

void MainScene::AddPine(const DirectX::XMFLOAT3& pos)
{
    auto pine = std::make_unique<Pine>();
    pine->SetWorldPosition(pos);
    pine->SetWorldScale(XMFLOAT3(5.f, 5.f, 5.f));
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
