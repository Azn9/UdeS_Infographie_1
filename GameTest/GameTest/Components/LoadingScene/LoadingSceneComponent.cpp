#include "LoadingSceneComponent.h"

#include "Api/Public/GameHost.h"
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/Input/Input.h"
#include "Api/Public/Util/Sound/SoundManager.h"
#include "Core/Public/Util/Time.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Scenes/MainScene.h"

void LoadingSceneComponent::Initialize()
{
    // Load scene in the background
    loadingThread = std::thread([this]
    {
        const auto mainScenePtr = MainScene::GetInstancePtr();
        mainScenePtr->Initialize();

        isLoaded = true;

        callback();
    });

    loadingThread.detach();
}

void LoadingSceneComponent::Update()
{
    if (!isLoaded)
        return;

    if (wantToChangeScene)
    {
        SoundManager::GetInstance().StopAllSounds();
        //SoundManager::GetInstance().LoadAndPlay("sounds/music/music2.wav"); // TODO

        PM3D_API::GameHost::GetInstance()->SetScene(MainScene::GetInstancePtr());
    }
}

void LoadingSceneComponent::Start()
{
    wantToChangeScene = true;
}
