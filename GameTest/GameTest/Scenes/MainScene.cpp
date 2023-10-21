#include "MainScene.h"

#include <iostream>
#include <memory>
#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/BasicCube.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "../../../PM3DApi/Api/Public/Light/PointLight.h"
#include "../../../PM3DApi/Api/Public/Shader/Basic/DefaultShader.h"
#include "GameTest/Components/CameraMoverComponent.h"

void MainScene::Initialize()
{
    std::cout << "MainScene::Initialize()" << std::endl;

    // Add main camera
    auto mainCamera = std::make_unique<Camera>(
        "Main camera",
        Camera::PERSECTIVE,
        XMFLOAT3(0.0f, 5.0f, -1.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarPlane(1000.0f);

    auto cameraMover = std::make_unique<CameraMoverComponent>();
    mainCamera->AddComponent(std::move(cameraMover));

    SetMainCamera(std::move(mainCamera));

    auto redLight = std::make_unique<PointLight>(
        "Red light",
        XMFLOAT3(-2.5f, 20.0f, -2.5f),
        XMFLOAT3(1.0f, 0.0f, 0.0f)
    );

    redLight->SetIntensity(1.0f);
    redLight->Initialize();
    AddLight(std::move(redLight));

    auto blueLight = std::make_unique<PointLight>(
        "Blue light",
        XMFLOAT3(2.5f, 20.0f, -2.5f),
        XMFLOAT3(0.0f, 1.0f, 0.0f)
    );
    blueLight->SetIntensity(1.0f);
    blueLight->Initialize();
    AddLight(std::move(blueLight));

    auto greenLight = std::make_unique<PointLight>(
        "Green light",
        XMFLOAT3(0.0f, 20.0f, 1.83f),
        XMFLOAT3(0.0f, 0.0f, 1.0f)
    );
    greenLight->SetIntensity(1.0f);
    greenLight->Initialize();
    AddLight(std::move(greenLight));

    auto shader = std::make_unique<PM3D_API::DefaultShader>(L"NewShader.fx");

    // Add a cube
    auto cube = std::make_unique<BasicCube>(
        "A cube",
        std::move(shader),
        XMFLOAT3(0.0f, 0.0f, 0.0f),
        XMFLOAT3(0.0f, 0.0f, 0.0f),
        XMFLOAT3(10.0f, 0.1f, 10.0f)
    );
    cube->Initialize();
    AddChild(std::move(cube));
}
