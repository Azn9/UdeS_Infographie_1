#include "MainScene.h"

#include <memory>

#include "Api/Public/Camera/Camera.h"

#include "Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Light/AmbiantLight.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Objects/Pine.h"

#include "GameTest/UI/GameUI.h"
#include "GameTest/Objects/MainScene/Map.h"
#include <GameTest/Objects/Skier.h>
#include <Api/Public/Component/Basic/Physics/BoxCollider.h>
#include <Api/Public/Component/Basic/Physics/SkierCollider.h>
#include <GameTest/Objects/Right_Ski.h>
#include <GameTest/Objects/Left_ski.h>
#include <GameTest/Components/JointsBreakerComponent.h>
#include <GameTest/Components/ToggleOnSkierComponent.h>
#include <GameTest/Components/SkierDeleter.h>
#include <GameTest/Components/SkierSpawner.h>

#include "Api/Private/Light/Shadow/ShadowProcessor.h"
#include "Api/Public/Util/Sound/SoundManager.h"
#include "GameTest/Objects/Sphere/Sphere.h"

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
        PM3D_API::Camera::PERSPECTIVE,
        XMFLOAT3(-2.0f, -50.0f, 50.0f),
        XMVectorSet(-2.0f, -50.0f, 80.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(200000.0f);
    mainCamera->AddComponent(std::make_unique<CameraFollowComponent>());
    mainCamera->SetClearColor(XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
    SetMainCamera(std::move(mainCamera));
}

void MainScene::InitializeLights()
{
	auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
		"Directional light",
		XMFLOAT3(1.0f, -1.0f, 0.0f)
	);
	directionalLight->SetIntensity(1.0f);
	directionalLight->Initialize();
	AddLight(std::move(directionalLight));
}

void MainScene::InitializeObjects()
{

    // ============= Base elements from the loading scene =====
    auto objects = std::make_unique<GameObject>("Objects");
    objects->Initialize();
    const auto objectsPtr = objects.get();
    auto objectsShader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    objects->AddComponent(
        std::make_unique<PM3D_API::MeshRenderer>(std::move(objectsShader), "LoadingScene/objects.obj"));
    AddChild(std::move(objects));

    auto objectsRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto objectsRigidbodyPtr = objectsRigidbody.get();
    objectsPtr->AddComponent(std::move(objectsRigidbody));
    objectsRigidbodyPtr->Initialize();

    auto objectsMeshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto objectsMeshColliderPtr = objectsMeshCollider.get();
    objectsPtr->AddComponent(std::move(objectsMeshCollider));
    objectsMeshColliderPtr->Initialize();

    // ============= Add the map =============

    auto map = std::make_unique<Map>();
    const auto mapPtr = map.get();
    AddChild(std::move(map));
    mapPtr->Initialize();

    // === Add skybox ===
    auto skybox = std::make_unique<GameObject>("Skybox");
    skybox->SetWorldScale({10000.f,10000.f,10000.f});
    skybox->Initialize();
    auto skyShader = std::make_unique<PM3D_API::DefaultShader>(L"shader/SkyShader.fx");
    auto skyRenderer = std::make_unique<PM3D_API::MeshRenderer>(std::move(skyShader), "skybox.obj");
    skyRenderer->SetIgnoreCulling(true);
    skybox->AddComponent(std::move(skyRenderer));
    AddChild(std::move(skybox));


    auto sphere = std::make_unique<Sphere>();
    spherePtr = sphere.get();
    AddChild(std::move(sphere));
    spherePtr->Initialize();

    auto skierSpawner = std::make_unique<SkierSpawner>();
    const auto skierSpawnerPtr = skierSpawner.get();
    AddComponent(std::move(skierSpawner));
    skierSpawnerPtr->Initialize();
    skierSpawnerPtr->Update();

    
    auto shadowProcessor = std::make_unique<ShadowProcessor>();
    shadowProcessor->Initialize();
    shadowProcessor->SetScene(this);
    AddComponent(std::move(shadowProcessor));
}

	void MainScene::InitializeUI()
	{
		Scene::InitializeUI(); // Init the base canvas

    auto gameUI = std::make_unique<GameUI>();
    const auto gameUIPtr = gameUI.get();
    AddUiChild(std::move(gameUI));
    gameUIPtr->Initialize();

    if (
        const auto loadRes = SoundManager::GetInstance().loadSound(
            "sounds/effects/toung.wav",
            &SoundManager::GetInstance().toungBuffer
        );
        !loadRes
    )
    {
        std::cerr << "Failed to load sound toung.wav" << std::endl;
    }
    else
    {
        std::cout << "Loaded sound toung.wav" << std::endl;
    }
}

