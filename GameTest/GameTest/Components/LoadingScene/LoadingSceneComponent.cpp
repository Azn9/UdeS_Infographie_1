#include "LoadingSceneComponent.h"

#include "Api/Public/GameHost.h"
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/Input/Input.h"
#include "Api/Public/Util/Sound/SoundManager.h"
#include "Core/Public/Util/Time.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Event/GameStartEvent.h"
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

        if (SoundManager::GetInstance().music2Buffer)
            SoundManager::GetInstance().Play(SoundManager::GetInstance().music2Buffer);

        PM3D_API::GameHost::GetInstance()->SetScene(MainScene::GetInstancePtr());
        PM3D_API::EventSystem::Publish(GameStartEvent());
    }
}

void LoadingSceneComponent::Start()
{
    wantToChangeScene = true;
}
