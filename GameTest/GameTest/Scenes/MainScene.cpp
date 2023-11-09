#include "MainScene.h"

#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/EventSystem/EventSystem.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "GameTest/TimeScaleTest.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/UI/TestUIObject.h"
#include <Heightmap.h>

#include "Component/Basic/Physics/MeshCollider.h"
#include "Component/Basic/Physics/SphereCollider.h"
#include "GameObject/Basic/BasicSphere.h"

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
        XMVectorSet(0.0f, -5.0f, 15.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarPlane(1000.0f);
    mainCamera->AddComponent(std::make_unique<CameraFollowComponent>());
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
    auto map = std::make_unique<Heightmap>();
    map->SetWorldPosition(XMFLOAT3(0.0f, -10.0f, 10.0f));
    map->SetWorldScale(XMFLOAT3(2.f, 2.f, 2.f));
    map->Initialize();

    auto mapRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto mapRigidbodyPtr = mapRigidbody.get();
    map->AddComponent(std::move(mapRigidbody));
    mapRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto meshColliderPtr = meshCollider.get();
    map->AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();

    AddChild(std::move(map));

    // ============= Add a sphere =============
    auto sphere = std::make_unique<PM3D_API::BasicSphere>("Sphere");
    sphere->SetWorldPosition(XMFLOAT3(0.0f, 15.0f, 10.0f));
    sphere->SetWorldScale(XMFLOAT3(.25f, .25f, .25f));
    sphere->Initialize();

    auto sphereRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto sphereRigidbodyPtr = sphereRigidbody.get();
    sphere->AddComponent(std::move(sphereRigidbody));
    sphereRigidbodyPtr->Initialize();

    auto sphereCollider = std::make_unique<PM3D_API::SphereCollider>(physicsResolver->GetDefaultMaterial());
    const auto sphereColliderPtr = sphereCollider.get();
    sphere->AddComponent(std::move(sphereCollider));
    sphereColliderPtr->Initialize();
    
    GetMainCamera()->GetComponent<CameraFollowComponent>()->SetObjectToFollow(sphere.get());

    AddChild(std::move(sphere));
    GetMainCamera();

    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));
    PM3D::Time::GetInstance().SetTimeScale(0.0f);
}

void MainScene::InitializeUI()
{
    Scene::InitializeUI(); // Init the base canvas

    auto testUiObject = std::make_unique<TestUIObject>(
        "TestUIObject",
        XMFLOAT2(0.5f, 0.5f),
        uiCanvas.get(),
        PM3D_API::ScaleMode::BOTH_RELATIVE,
        XMFLOAT2(0.0f, 0.0f),
        PM3D_API::Anchor::MIDDLE,
        PM3D_API::Anchor::MIDDLE
    );
    testUiObject->Initialize();
    AddUiChild(std::move(testUiObject));
}
