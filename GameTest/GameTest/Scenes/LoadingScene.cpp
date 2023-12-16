#include "LoadingScene.h"

#include <thread>

#include "MainScene.h"
#include "Api/Public/Util/Sound/SoundManager.h"
#include "GameTest/Objects/LoadingScene/CustomCube.h"
#include "GameTest/Objects/LoadingScene/CustomPlane.h"
#include "GameTest/Components/LightMoverComponent.h"
#include "GameTest/Components/LoadingScene/LoadingSceneComponent.h"
#include "GameTest/Components/SnowMover.h"
#include "GameTest/Components/LoadingScene/CameraMoverComponent.h"
#include "GameTest/UI/FadeWhiteComponent.h"
#include "GameTest/UI/Menus/MainMenuUI.h"

#include "Api/Private/Light/Shadow/ShadowProcessor.h"
#include "GameTest/Components/TournerComponent.h"
#include "GameTest/Objects/Sphere/SphereRenderer.h"
#include "GameTest/Objects/Sphere/SphereShader.h"

void LoadingScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSPECTIVE,
        XMFLOAT3(-2.0f, -45.0f, 50.0f),
        XMVectorSet(6.0f, -45.0f, 66.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(20000.0f);
    mainCamera->SetClearColor(XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
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
    auto ground = std::make_unique<CustomPlane>();
    ground->Initialize();
    AddChild(std::move(ground));

    auto objects = std::make_unique<GameObject>("Objects");
    objects->Initialize();
    auto objectsShader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    objects->AddComponent(
        std::make_unique<PM3D_API::MeshRenderer>(std::move(objectsShader), "LoadingScene/objects.obj"));
    AddChild(std::move(objects));

    // 20 -52 88
    auto otherSnowball = std::make_unique<GameObject>("OS");
    const auto otherSnowballPtr = otherSnowball.get();
    AddChild(std::move(otherSnowball));
    auto sphereShader1 = std::make_unique<SphereShader>(L"shader/SphereShader.fx");
    auto sphereRenderer1 = std::make_unique<SphereRenderer>(std::move(sphereShader1));
    const auto sphereRendererPtr1 = sphereRenderer1.get();
    otherSnowballPtr->AddComponent(std::move(sphereRenderer1));
    sphereRendererPtr1->Initialize();
    otherSnowballPtr->SetWorldPosition(XMFLOAT3(20.f, -52.f, 88.f));
    otherSnowballPtr->SetWorldScale(XMFLOAT3(1.f, 1.f, 1.f));
    otherSnowballPtr->AddComponent(std::make_unique<SnowMover>());
    otherSnowballPtr->AddComponent(std::make_unique<TournerComponent>());

    auto shadowProcessor = std::make_unique<ShadowProcessor>();
    shadowProcessor->Initialize();
    shadowProcessor->SetScene(this);
    AddComponent(std::move(shadowProcessor));

    // === Add skybox ===
    auto skybox = std::make_unique<GameObject>("Skybox");
    skybox->SetWorldScale({10000.f, 10000.f, 10000.f});
    skybox->Initialize();
    auto skyShader = std::make_unique<PM3D_API::DefaultShader>(L"shader/SkyShader.fx");
    auto skyRenderer = std::make_unique<PM3D_API::MeshRenderer>(std::move(skyShader), "skybox.obj");
    skyRenderer->SetIgnoreCulling(true);
    skybox->AddComponent(std::move(skyRenderer));
    AddChild(std::move(skybox));

    auto sphere = std::make_unique<GameObject>("Sphere");
    sphere->Initialize();
    sphere->SetWorldScale(XMFLOAT3(1.f, 1.f, 1.f));
    sphere->SetWorldPosition(XMFLOAT3(6.f, -50.f, 66.f));
    const auto spherePtr = sphere.get();
    AddChild(std::move(sphere));
    auto sphereShader = std::make_unique<SphereShader>(L"shader/SphereShader.fx");
    auto sphereRenderer = std::make_unique<SphereRenderer>(std::move(sphereShader));
    const auto sphereRendererPtr = sphereRenderer.get();
    spherePtr->AddComponent(std::move(sphereRenderer));
    sphereRendererPtr->Initialize();
}

void LoadingScene::InitializeUI()
{
    Scene::InitializeUI();

    if (
        const auto loadRes = SoundManager::GetInstance().loadSound(
            "sounds/ui/rollover1.wav",
            &SoundManager::GetInstance().uiRollover1Buffer
        );
        !loadRes
    )
    {
        std::cerr << "Failed to load sound rollover1.wav" << std::endl;
    }
    else
    {
        std::cout << "Loaded sound rollover1.wav" << std::endl;
    }

    if (
        const auto loadRes = SoundManager::GetInstance().loadSound(
            "sounds/ui/click1.wav",
            &SoundManager::GetInstance().uiClick1Buffer
        );
        !loadRes
    )
    {
        std::cerr << "Failed to load sound click1.wav" << std::endl;
    }
    else
    {
        std::cout << "Loaded sound click1.wav" << std::endl;
    }

    if (
        const auto loadRes = SoundManager::GetInstance().loadSound(
            "sounds/music/music1.wav",
            &SoundManager::GetInstance().music1Buffer
        );
        !loadRes
    )
    {
        std::cerr << "Failed to load sound music1.wav" << std::endl;
    }
    else
    {
        std::cout << "Loaded sound music1.wav" << std::endl;
        SoundManager::GetInstance().Play(SoundManager::GetInstance().music1Buffer);
    }

    auto mainMenuUi = std::make_unique<MainMenuUI>();
    const auto mainMenuUiPtr = mainMenuUi.get();
    AddUiChild(std::move(mainMenuUi));
    mainMenuUiPtr->Initialize();

    auto whiteFadeObject = std::make_unique<FadeWhiteComponent>();
    const auto whiteFadeObjectPtr = whiteFadeObject.get();
    AddUiChild(std::move(whiteFadeObject));
    whiteFadeObjectPtr->Initialize();
    whiteFadeObjectPtr->SetAlpha(1.0f);
    whiteFadeObjectPtr->SetSpeed(1.f / 3.f);
    whiteFadeObjectPtr->SetToTransparent(true);
    whiteFadeObjectPtr->SetFading(true);
}
