#include "SRScene.h"

#include "Api/Public/GameObject/Basic/BasicSphere.h"
#include "GameTest/Components/LoadingScene/CameraMoverComponent.h"
#include "GameTest/Objects/Sphere/Sphere.h"

void SRScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSPECTIVE,
        DirectX::XMFLOAT3(0.0f, 5.0f, -1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.f, 1.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
    mainCamera->SetClearColor(DirectX::XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
    mainCamera->AddComponent(std::make_unique<CameraMoverComponent>());
    SetMainCamera(std::move(mainCamera));
}

void SRScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void SRScene::InitializeObjects()
{
    auto sphere = std::make_unique<Sphere>();
    const auto spherePtr = sphere.get();
    AddChild(std::move(sphere));
    spherePtr->Initialize();
}
