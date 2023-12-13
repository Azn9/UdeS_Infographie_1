#include "MainMenuUI.h"

#include "Api/Public/UI/Button.h"
#include "Core/Public/Core/Moteur.h"
#include "Core/Public/Core/MoteurWindows.h"

void MainMenuUI::Initialize()
{
    constexpr auto expectedScreenSize = DirectX::XMFLOAT2(1920.f, 1080.f);

    auto playButton = std::make_unique<Button>(
        L"sprite/ui/large_btn_base.dds",
        L"sprite/ui/large_btn_hover.dds",
        L"sprite/ui/large_btn_pressed.dds",
        1311.f / expectedScreenSize.x,
        236.f / expectedScreenSize.y,
        500.f / expectedScreenSize.x,
        130.f / expectedScreenSize.y,
        "Jouer",
        []
        {
            // TODO
        }
    );
    const auto playButtonPtr = playButton.get();
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
        "",
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
        "",
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
        "",
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
        "",
        []
        {
            std::cout << "Quit button pressed" << std::endl;
            PM3D::CMoteurWindows::GetInstance().wantExit = true;
        }
    );
    const auto quitButtonPtr = quitButton.get();
    canvas->AddChild(std::move(quitButton));
    quitButtonPtr->Initialize();
}

void MainMenuUI::DrawSelf() const
{
    UIObject::DrawSelf();
}
