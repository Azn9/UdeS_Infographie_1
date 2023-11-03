#include "MainScene.h"

#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/BoxCollider.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/PlaneCollider.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/BasicCube.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/BasicSphere.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "../../../PM3DApi/Api/Public/Light/PointLight.h"
#include "../../../PetitMoteur3D/Core/Public/Mesh/FastobjChargeur.h"
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
    auto plane = std::make_unique<Heightmap>();
    plane->SetWorldPosition(XMFLOAT3(0.0f, -10.0f, 10.0f));
    plane->SetWorldScale(XMFLOAT3(.5f, .5f, .5f));
    plane->Initialize();

    auto planeRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto planeRigidbodyPtr = planeRigidbody.get();
    plane->AddComponent(std::move(planeRigidbody));
    planeRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto meshColliderPtr = meshCollider.get();
    plane->AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();

    AddChild(std::move(plane));

    // ============= Add a cube =============
    auto cube = std::make_unique<PM3D_API::BasicCube>("Cube");
    cube->SetWorldPosition(XMFLOAT3(0.0f, 15.0f, 10.0f));
    cube->SetWorldScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
    cube->Initialize();

    auto cubeRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto cubeRigidbodyPtr = cubeRigidbody.get();
    cube->AddComponent(std::move(cubeRigidbody));
    cubeRigidbodyPtr->Initialize();

    auto cubeCollider = std::make_unique<PM3D_API::BoxCollider>(physicsResolver->GetDefaultMaterial());
    const auto cubeColliderPtr = cubeCollider.get();
    cube->AddComponent(std::move(cubeCollider));
    cubeColliderPtr->Initialize();
    
    AddChild(std::move(cube));

    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));

    PM3D::Time::GetInstance().SetTimeScale(0.0f);
}
