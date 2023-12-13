#include "MainMenuUI.h"

#include "Api/Public/Component/Basic/Render/2D/TextRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/UI/Button.h"
#include "Api/Public/UI/DisabledButton.h"
#include "Core/Public/Core/Moteur.h"
#include "Core/Public/Core/MoteurWindows.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Components/LoadingScene/LoadingSceneComponent.h"
#include "GameTest/Event/GameOverEvent.h"
#include "GameTest/Event/GameStartEvent.h"

void MainMenuUI::Initialize()
{
    constexpr auto expectedScreenSize = DirectX::XMFLOAT2(1920.f, 1080.f);

    const auto iceSnowFontLoader = PM3D_API::FontLoader("sprite/font/icesnow.fnt", L"sprite/font/icesnow.dds");

    auto disabledPlayButton = std::make_unique<DisabledButton>(
        L"sprite/ui/large_btn_disabled.dds",
        1311.f / expectedScreenSize.x,
        236.f / expectedScreenSize.y,
        500.f / expectedScreenSize.x,
        130.f / expectedScreenSize.y,
        iceSnowFontLoader,
        "CHARGEMENT",
        DirectX::XMFLOAT2(30.0f, 25.0f),
        0.3f
    );
    disabledPlayButtonPtr = disabledPlayButton.get();
    canvas->AddChild(std::move(disabledPlayButton));
    disabledPlayButtonPtr->Initialize();

    auto playButton = std::make_unique<Button>(
        L"sprite/ui/large_btn_base.dds",
        L"sprite/ui/large_btn_hover.dds",
        L"sprite/ui/large_btn_pressed.dds",
        1311.f / expectedScreenSize.x,
        236.f / expectedScreenSize.y,
        500.f / expectedScreenSize.x,
        130.f / expectedScreenSize.y,
        iceSnowFontLoader,
        "JOUER",
        DirectX::XMFLOAT2(70.0f, 20.0f),
        0.4f,
        [&]
        {
            loadingSceneComponentPtr->Start();
        }
    );
    playButton->SetAlpha(0.f);
    playButtonPtr = playButton.get();
    canvas->AddChild(std::move(playButton));
    playButtonPtr->Initialize();

    auto leaderboardButton = std::make_unique<Button>(
        L"sprite/ui/small_btn_base.dds",
        L"sprite/ui/small_btn_hover.dds",
        L"sprite/ui/small_btn_pressed.dds",
        1311.f / expectedScreenSize.x,
        109.f / expectedScreenSize.y,
        120.f / expectedScreenSize.x,
        120.f / expectedScreenSize.y,
        []
        {
            // TODO
        }
    );
    const auto leaderboardButtonPtr = leaderboardButton.get();
    canvas->AddChild(std::move(leaderboardButton));
    leaderboardButtonPtr->Initialize();

    auto creditsButton = std::make_unique<Button>(
        L"sprite/ui/small_btn_base.dds",
        L"sprite/ui/small_btn_hover.dds",
        L"sprite/ui/small_btn_pressed.dds",
        1438.f / expectedScreenSize.x,
        109.f / expectedScreenSize.y,
        120.f / expectedScreenSize.x,
        120.f / expectedScreenSize.y,
        []
        {
            // TODO
        }
    );
    const auto creditsButtonPtr = creditsButton.get();
    canvas->AddChild(std::move(creditsButton));
    creditsButtonPtr->Initialize();

    auto settingsButton = std::make_unique<Button>(
        L"sprite/ui/small_btn_base.dds",
        L"sprite/ui/small_btn_hover.dds",
        L"sprite/ui/small_btn_pressed.dds",
        1565.f / expectedScreenSize.x,
        109.f / expectedScreenSize.y,
        120.f / expectedScreenSize.x,
        120.f / expectedScreenSize.y,
        []
        {
            // TODO
        }
    );
    const auto settingsButtonPtr = settingsButton.get();
    canvas->AddChild(std::move(settingsButton));
    settingsButtonPtr->Initialize();

    auto quitButton = std::make_unique<Button>(
        L"sprite/ui/small_btn_base.dds",
        L"sprite/ui/small_btn_hover.dds",
        L"sprite/ui/small_btn_pressed.dds",
        1692.f / expectedScreenSize.x,
        109.f / expectedScreenSize.y,
        120.f / expectedScreenSize.x,
        120.f / expectedScreenSize.y,
        []
        {
            std::cout << "Quit button pressed" << std::endl;
            PM3D::CMoteurWindows::GetInstance().wantExit = true;
        }
    );
    const auto quitButtonPtr = quitButton.get();
    canvas->AddChild(std::move(quitButton));
    quitButtonPtr->Initialize();

    auto obj = std::make_unique<PM3D_API::UIObject>(
        "Text",
        XMFLOAT2(1477.0f / expectedScreenSize.x, 241.0f / expectedScreenSize.y),
        XMFLOAT2(222.0f / expectedScreenSize.x, 800.0f / expectedScreenSize.y),
        0.f,
        true,
        true
    );
    const auto objPtr = obj.get();
    auto textRenderer = std::make_unique<PM3D_API::TextRenderer>(
        iceSnowFontLoader,
        "PROJECT SNOWBALL",
        DirectX::XMFLOAT2(0.0f, 0.0f),
        XMFLOAT3(1.0f, 1.0f, 1.0f),
        0.7f
    );
    const auto textRendererPtr = textRenderer.get();
    obj->AddComponent(std::move(textRenderer));
    canvas->AddChild(std::move(obj));
    objPtr->Initialize();
    textRendererPtr->Initialize();

    auto loadingSceneComponent = std::make_unique<LoadingSceneComponent>([this]
    {
        this->LoadComplete();
    });
    loadingSceneComponentPtr = loadingSceneComponent.get();
    canvas->GetScene()->AddComponent(std::move(loadingSceneComponent));
    loadingSceneComponentPtr->Initialize();
}

void MainMenuUI::DrawSelf() const
{
    if (!displayed) return;

    UIObject::DrawSelf();
}

void MainMenuUI::LoadComplete() const
{
    disabledPlayButtonPtr->SetAlpha(0.f);
    playButtonPtr->SetAlpha(1.f);
}
