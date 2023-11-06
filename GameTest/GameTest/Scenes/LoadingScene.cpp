#include "LoadingScene.h"

#include <thread>

#include "MainScene.h"
#include "GameTest/CustomCube.h"
#include "GameTest/CustomPlane.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/LightMoverComponent.h"
#include "GameTest/Components/SnowMover.h"

void LoadingScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSECTIVE,
        XMFLOAT3(0.0f, 10.0f, -15.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarPlane(100.0f);
    //mainCamera->AddComponent(std::make_unique<CameraMoverComponent>());
    SetMainCamera(std::move(mainCamera));
}

void LoadingScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
       "Directional light",
       XMFLOAT3(-1.0f, -1.0f, 0.0f)
   );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void LoadingScene::InitializeObjects()
{
    // ============= Add a plane =============
    auto plane = std::make_unique<CustomPlane>();
    plane->SetWorldScale(XMFLOAT3(100.0f, 1.0f, 100.0f));
    plane->SetWorldScale(XMFLOAT3(10.0f, 1.0f, 10.0f));
    plane->Initialize();
    AddChild(std::move(plane));

    auto cube = std::make_unique<CustomCube>();
    cube->SetWorldPosition(XMFLOAT3(0, 1.0f, 0.0f));
    cube->SetWorldScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
    cube->Initialize();

    cube->AddComponent(std::make_unique<LightMoverComponent>(0.f));
    cube->AddComponent(std::make_unique<SnowMover>());
    
    AddChild(std::move(cube));
}
