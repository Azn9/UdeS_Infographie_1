#include "MainMenuUI.h"

#include "Api/Public/Component/Basic/Render/2D/TextRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/Input/Input.h"
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

    auto creditsObj = std::make_unique<UIObject>("Credits", XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), 0.0f, true,
                                                 true);
    auto creditsRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"sprite/ui/credits.dds");
    creditsObj->SetAlpha(0.f);
    creditsObjectPtr = creditsObj.get();
    const auto creditsRendererPtr = creditsRenderer.get();
    canvas->AddChild(std::move(creditsObj));
    creditsObjectPtr->AddComponent(std::move(creditsRenderer));
    creditsRendererPtr->Initialize();

    auto paywallObj = std::make_unique<UIObject>("paywall", XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), 0.0f, true,
                                                 true);
    auto paywallRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"sprite/ui/paywall.dds");
    paywallObj->SetAlpha(0.f);
    paywallObjectPtr = paywallObj.get();
    const auto paywallRendererPtr = paywallRenderer.get();
    canvas->AddChild(std::move(paywallObj));
    paywallObjectPtr->AddComponent(std::move(paywallRenderer));
    paywallRendererPtr->Initialize();

    Input::GetInstance().AddKeyPressedListener([&](const KeyCode& key_code)
    {
        if (key_code == KeyCode::ESCAPE && (creditsDisplayed || paywallDisplayed))
        {
            creditsDisplayed = false;
            paywallDisplayed = false;

            creditsObjectPtr->SetAlpha(0.f);
            paywallObjectPtr->SetAlpha(0.f);
            SetAlpha(1.f);

            for (const auto component : allComponents)
            {
                if (component)
                    component->SetAlpha(alpha);
            }

            disabledPlayButtonPtr->SetAlpha(loaded ? 0.f : 1.f);
            playButtonPtr->SetAlpha(loaded ? 1.f : 0.f);
        }
    });

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
    allComponents.push_back(disabledPlayButtonPtr);
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
    allComponents.push_back(playButtonPtr);
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
        L"sprite/ui/classement_btn.dds",
        DirectX::XMFLOAT2(0.0f, 0.0f),
        [&]
        {
            paywallObjectPtr->SetAlpha(1.f);
            paywallDisplayed = true;
            SetAlpha(0.f);
        }
    );
    const auto leaderboardButtonPtr = leaderboardButton.get();
    allComponents.push_back(leaderboardButtonPtr);
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
        L"sprite/ui/credits_btn.dds",
        DirectX::XMFLOAT2(0.0f, 0.0f),
        [&]
        {
            creditsObjectPtr->SetAlpha(1.f);
            creditsDisplayed = true;
            SetAlpha(0.f);
        }
    );
    const auto creditsButtonPtr = creditsButton.get();
    allComponents.push_back(creditsButtonPtr);
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
        L"sprite/ui/settings_btn.dds",
        DirectX::XMFLOAT2(0.0f, 0.0f),
        [&]
        {
            paywallObjectPtr->SetAlpha(1.f);
            paywallDisplayed = true;
            SetAlpha(0.f);
        }
    );
    const auto settingsButtonPtr = settingsButton.get();
    allComponents.push_back(settingsButtonPtr);
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
        L"sprite/ui/exit_btn.dds",
        DirectX::XMFLOAT2(0.0f, 0.0f),
        []
        {
            std::cout << "Quit button pressed" << std::endl;
            PM3D::CMoteurWindows::GetInstance().wantExit = true;
        }
    );
    const auto quitButtonPtr = quitButton.get();
    allComponents.push_back(quitButtonPtr);
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
    allComponents.push_back(objPtr);
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

void MainMenuUI::Draw()
{
    for (const auto component : allComponents)
    {
        if (component)
            component->SetAlpha(alpha);
    }

    if (!displayed || creditsDisplayed || paywallDisplayed) return;

    disabledPlayButtonPtr->SetAlpha(loaded ? 0.f : 1.f);
    playButtonPtr->SetAlpha(loaded ? 1.f : 0.f);

    DrawSelf();

    for (auto& component : components)
    {
        if (component)
            component->DrawSelf();
    }
}


void MainMenuUI::DrawSelf() const
{
    UIObject::DrawSelf();
}

void MainMenuUI::LoadComplete()
{
    loaded = true;

    if (!displayed || creditsDisplayed || paywallDisplayed) return;

    disabledPlayButtonPtr->SetAlpha(0.f);
    playButtonPtr->SetAlpha(1.f);
}
