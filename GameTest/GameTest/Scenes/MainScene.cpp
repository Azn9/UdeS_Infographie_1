﻿#include "MainScene.h"

#include <iostream>
#include <memory>
#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/BasicCube.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/BasicPlane.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "../../../PM3DApi/Api/Public/Light/PointLight.h"
#include "../../../PM3DApi/Api/Public/Light/SpotLight.h"
#include "../../../PM3DApi/Api/Public/Shader/Basic/DefaultShader.h"
#include "GameTest/CustomPlane.h"
#include "GameTest/TimeScaleTest.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/LightMoverComponent.h"

void MainScene::Initialize()
{
    std::cout << "MainScene::Initialize()" << std::endl;

    // Add main camera
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSECTIVE,
        XMFLOAT3(0.0f, 5.0f, -10.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarPlane(1000.0f);
    SetMainCamera(std::move(mainCamera));

    /*auto spotlight = std::make_unique<PM3D_API::SpotLight>(
        "SpotLight",
        XMFLOAT3(-6.0f, 6.0f, 0.0f),
        XMFLOAT3(6.0f, -6.0f, 0.0f),
        XMFLOAT3(1.0f, 1.0f, 1.0f),
        0.0001f,
        12.0f * XM_PI / 180.0f,
        15.0f * XM_PI / 180.0f
    );
    spotlight->Initialize();
    AddLight(std::move(spotlight));*/

    auto pointLight = std::make_unique<PM3D_API::PointLight>(
        XMFLOAT3(0.0f, 5.0f, 0.0f),
        XMFLOAT3(1.0f, 1.0f, 1.0f)
    );
    
    auto lightMoverComponent = std::make_unique<LightMoverComponent>(0);
    pointLight->AddComponent(std::move(lightMoverComponent));
    
    pointLight->SetIntensity(2.0f);
    pointLight->Initialize();
    AddLight(std::move(pointLight));

    //auto shader = std::make_unique<PM3D_API::DefaultShader>(L"Test1.fx");

    // Add a cube
    auto plane = std::make_unique<CustomPlane>();
    plane->SetWorldScale(XMFLOAT3(10.0f, 1.0f, 10.0f));
    plane->Initialize();
    AddChild(std::move(plane));


    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));
}
