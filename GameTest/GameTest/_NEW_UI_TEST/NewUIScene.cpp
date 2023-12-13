#include "NewUIScene.h"

#include "Api/Public/Component/Basic/Render/2D/TextRenderer.h"
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "Api/Public/Shader/Basic/DefaultShader.h"
#include "Api/Public/UI/Button.h"
#include "Api/Public/Util/Font/FontLoader.h"
#include "Api/Public/Util/Sound/SoundManager.h"
#include "GameTest/Objects/LoadingScene/CustomPlane.h"
#include "GameTest/UI/FadeWhiteComponent.h"
#include "GameTest/UI/Menus/MainMenuUI.h"

using namespace DirectX;

void NewUIScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSPECTIVE,
        XMFLOAT3(-2.0f, -50.0f, 50.0f),
        XMVectorSet(6.0f, -50.0f, 66.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
    mainCamera->SetClearColor(XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
    SetMainCamera(std::move(mainCamera));
}

void NewUIScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        XMFLOAT3(-1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void NewUIScene::InitializeObjects()
{
    auto ground = std::make_unique<CustomPlane>();
    ground->Initialize();
    AddChild(std::move(ground));

    auto objects = std::make_unique<GameObject>("Objects");
    objects->Initialize();
    auto objectsShader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    objects->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(objectsShader), "LoadingScene/objects.obj"));
    AddChild(std::move(objects));
}

void NewUIScene::InitializeUI()
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
