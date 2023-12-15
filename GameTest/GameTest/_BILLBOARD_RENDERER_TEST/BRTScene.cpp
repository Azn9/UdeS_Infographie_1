#include "BRTScene.h"

#include "Api/Public/Component/Basic/Render/3D/BillboardRenderer.h"
#include "Api/Public/GameObject/Basic/BasicCube.h"
#include "GameTest/Components/LoadingScene/CameraMoverComponent.h"

void BRTScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSPECTIVE,
        DirectX::XMFLOAT3(-5.0f, 0.0f, 0.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
    mainCamera->SetClearColor(DirectX::XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
    mainCamera->AddComponent(std::make_unique<CameraMoverComponent>());
    SetMainCamera(std::move(mainCamera));
}

void BRTScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void BRTScene::InitializeObjects()
{
    auto cube = std::make_unique<PM3D_API::BasicCube>(
        "cube",
        XMFLOAT3(0.0f, -2.0f, 0.0f)
    );
    const auto cubePtr = cube.get();
    AddChild(std::move(cube));
    cubePtr->Initialize();

    {
        auto obj = std::make_unique<GameObject>(
            "test1",
            XMFLOAT3(0.0f, 0.0f, 0.0f)
        );
        obj->Initialize();
        const auto objPtr = obj.get();
        AddChild(std::move(obj));
        auto shader = std::make_unique<PM3D_API::SpriteShader>(L"shader/Sprite1.fx");
        auto billboardRenderer = std::make_unique<PM3D_API::BillboardRenderer>(
            std::move(shader),
            L"sprite/trees/pine.dds",
            PM3D_API::BillboardAlignment::NONE,
            XMFLOAT2(1 / 304.f, 3 / 880.f)
        );
        const auto billboardRendererPtr = billboardRenderer.get();
        objPtr->AddComponent(std::move(billboardRenderer));
        billboardRendererPtr->Initialize();
    }
}
