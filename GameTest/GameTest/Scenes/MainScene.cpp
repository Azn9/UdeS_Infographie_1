#include "MainScene.h"

#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/BoxCollider.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/PlaneCollider.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/BillboardRenderer.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/EventSystem/EventSystem.h"
#include "../../../PM3DApi/Api/Public/EventSystem/Basic/WindowResizeEvent.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/BasicCube.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/BasicSphere.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "../../../PM3DApi/Api/Public/Light/PointLight.h"
#include "../../../PM3DApi/Api/Public/Light/SpotLight.h"
#include "GameTest/CustomCube.h"
#include "GameTest/CustomPlane.h"
#include "GameTest/TimeScaleTest.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/LightMoverComponent.h"

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
        XMFLOAT3(0.0f, 10.0f, -15.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
    mainCamera->AddComponent(std::make_unique<CameraMoverComponent>());
    SetMainCamera(std::move(mainCamera));
}

void MainScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        XMFLOAT3(-1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void MainScene::InitializeObjects()
{
    // ============= Add a plane =============
    auto plane = std::make_unique<CustomPlane>();
    plane->SetWorldScale(XMFLOAT3(10.0f, 1.0f, 10.0f));
    plane->Initialize();

    auto planeRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto planeRigidbodyPtr = planeRigidbody.get();
    plane->AddComponent(std::move(planeRigidbody));
    planeRigidbodyPtr->Initialize();

    auto planeCollider = std::make_unique<PM3D_API::PlaneCollider>(physicsResolver->GetDefaultMaterial());
    const auto planeColliderPtr = planeCollider.get();
    plane->AddComponent(std::move(planeCollider));
    planeColliderPtr->Initialize();

    AddChild(std::move(plane));

    // ============= Add a cube =============

    for (int i = 0; i < 10; ++i)
    {
        auto cube = std::make_unique<CustomCube>();
        cube->SetWorldPosition(XMFLOAT3(i*2, 1.0f + i*2, 0.0f));
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
    }


    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));
    PM3D::Time::GetInstance().SetTimeScale(0.0f);
}
