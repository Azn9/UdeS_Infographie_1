#include "MainScene.h"

#include <memory>

#include "Camera/Camera.h"

#include "Component/Basic/Physics/BoxCollider.h"
#include "Component/Basic/Physics/SphereCollider.h"
#include "Component/Basic/Physics/PlaneCollider.h"
#include "Component/Basic/Physics/MeshCollider.h"
#include "Component/Basic/Physics/Rigidbody.h"

#include "Component/Basic/Render/MeshRenderer.h"
#include "GameObject/GameObject.h"
#include "GameObject/Basic/BasicCube.h"
#include "GameObject/Basic/BasicSphere.h"
#include "Light/AmbiantLight.h"
#include "Light/PointLight.h"

#include "Mesh/FastobjChargeur.h"

#include "GameTest/CustomPlane.h"
#include "GameTest/TimeScaleTest.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/LightMoverComponent.h"
#include <Heightmap.h>

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
    SetMainCamera(std::move(mainCamera));
}

void MainScene::InitializeLights()
{
    auto pointLight = std::make_unique<PM3D_API::PointLight>(
        XMFLOAT3(0.0f, 5.0f, 0.0f),
        XMFLOAT3(1.0f, 1.0f, 1.0f)
    );
    
    auto lightMoverComponent = std::make_unique<LightMoverComponent>(0);
    pointLight->AddComponent(std::move(lightMoverComponent));
    
    pointLight->SetIntensity(2.0f);
    pointLight->Initialize();
    AddLight(std::move(pointLight));
}

void MainScene::InitializeObjects()
{
    // ============= Add a plane =============
    auto map = std::make_unique<Heightmap>();
    map->SetWorldPosition(XMFLOAT3(0.0f, -10.0f, 10.0f));
    map->SetWorldScale(XMFLOAT3(.5f, .5f, .5f));
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

    // ============= Add a cube =============
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
    
    AddChild(std::move(sphere));

    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));

    PM3D::Time::GetInstance().SetTimeScale(0.0f);
}
