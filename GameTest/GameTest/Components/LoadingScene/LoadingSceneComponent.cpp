#include "LoadingSceneComponent.h"

#include "Api/Public/GameHost.h"
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/Input/Input.h"
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
    });
    loadingThread.detach();

    Input::AddKeyPressedListener([this](const KeyCode& key)
    {
        if (key == KeyCode::ENTER)
        {
            wantToChangeScene = true;
        }
    });

    {
        constexpr auto loadingTextSize = DirectX::XMFLOAT2(1200.f, 148.f);
        constexpr auto expectedScreenSize = DirectX::XMFLOAT2(1920.f, 1080.f);
        constexpr auto loadingTextPosition = DirectX::XMFLOAT2(
            216.f,
            864.f
        );
        constexpr auto positionPercent = DirectX::XMFLOAT2(
            loadingTextPosition.x / expectedScreenSize.x,
            loadingTextPosition.y / expectedScreenSize.y
        );
        constexpr auto scalePercent = DirectX::XMFLOAT2(
            loadingTextSize.x / expectedScreenSize.x,
            loadingTextSize.y / expectedScreenSize.y
        );

        auto titleTextComponent = std::make_unique<PM3D_API::UIObject>(
            "Title",
            scalePercent,
            positionPercent,
            0.f,
            true,
            true
        );
        const auto loadingTextComponentPtr = titleTextComponent.get();
        parentObject->GetScene()->GetUICanvas()->AddChild(std::move(titleTextComponent));
        loadingTextComponentPtr->Initialize();

        auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"ps_title.dds");
        const auto spriteRendererPtr = spriteRenderer.get();
        loadingTextComponentPtr->AddComponent(std::move(spriteRenderer));
        spriteRendererPtr->Initialize();
    }

    {
        constexpr auto loadingTextSize = DirectX::XMFLOAT2(988.f / 3.f, 124.f / 3.f);
        constexpr auto expectedScreenSize = DirectX::XMFLOAT2(1920.f, 1080.f);
        constexpr auto loadingTextPosition = DirectX::XMFLOAT2(
            1482.f,
            10.f
        );
        constexpr auto positionPercent = DirectX::XMFLOAT2(
            loadingTextPosition.x / expectedScreenSize.x,
            loadingTextPosition.y / expectedScreenSize.y
        );
        constexpr auto scalePercent = DirectX::XMFLOAT2(
            loadingTextSize.x / expectedScreenSize.x,
            loadingTextSize.y / expectedScreenSize.y
        );

        auto loadingTextComponent = std::make_unique<PM3D_API::UIObject>(
            "Loading text",
            scalePercent,
            positionPercent,
            0.f,
            true,
            true
        );
        loadingTextPtr = loadingTextComponent.get();
        const auto loadingTextComponentPtr = loadingTextComponent.get();
        parentObject->GetScene()->GetUICanvas()->AddChild(std::move(loadingTextComponent));
        loadingTextComponentPtr->Initialize();

        auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"loading_text.dds");
        const auto spriteRendererPtr = spriteRenderer.get();
        loadingTextComponentPtr->AddComponent(std::move(spriteRenderer));
        spriteRendererPtr->Initialize();
    }

    {
        constexpr auto loadingTextSize = DirectX::XMFLOAT2(360.f, 132.f);
        constexpr auto expectedScreenSize = DirectX::XMFLOAT2(1920.f, 1080.f);
        constexpr auto loadingTextPosition = DirectX::XMFLOAT2(
            1452.f,
            80.f
        );
        constexpr auto positionPercent = DirectX::XMFLOAT2(
            loadingTextPosition.x / expectedScreenSize.x,
            loadingTextPosition.y / expectedScreenSize.y
        );
        constexpr auto scalePercent = DirectX::XMFLOAT2(
            loadingTextSize.x / expectedScreenSize.x,
            loadingTextSize.y / expectedScreenSize.y
        );

        auto loadingTextComponent = std::make_unique<PM3D_API::UIObject>(
            "Play button",
            scalePercent,
            positionPercent,
            0.f,
            true,
            true
        );
        loadingTextComponent->SetAlpha(0.f);
        playButtonPtr = loadingTextComponent.get();
        const auto loadingTextComponentPtr = loadingTextComponent.get();
        parentObject->GetScene()->GetUICanvas()->AddChild(std::move(loadingTextComponent));
        loadingTextComponentPtr->Initialize();

        auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"playbutton.dds");
        const auto spriteRendererPtr = spriteRenderer.get();
        loadingTextComponentPtr->AddComponent(std::move(spriteRenderer));
        spriteRendererPtr->Initialize();
    }
}

void LoadingSceneComponent::Update()
{
    if (!isLoaded)
        return;

    if (wantToChangeScene)
    {
        PM3D_API::GameHost::GetInstance()->SetScene(MainScene::GetInstancePtr());
        return;
    }

    loadingTextPtr->SetAlpha(Util::Lerp(loadingTextPtr->GetAlpha(), 0, 5.f * PM3D::Time::GetInstance().GetUpdateDeltaTime()));
    playButtonPtr->SetAlpha(Util::Lerp(playButtonPtr->GetAlpha(), 1, 5.f * PM3D::Time::GetInstance().GetUpdateDeltaTime()));

    if (Input::IsKeyPressed(KeyCode::ARROW_DOWN))
    {
        // TODO : menus
    }
}
