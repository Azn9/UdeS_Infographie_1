#include "PauseComponent.h"

#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/Input/Input.h"
#include "Api/Public/Util/Util.h"
#include "Core/Public/Util/Time.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Event/GameOverEvent.h"
#include "GameTest/Event/PauseEvent.h"

void PauseComponent::Initialize()
{
    UIObject::Initialize();

    {
        auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"pause.dds");
        pauseRenderer = spriteRenderer.get();
        AddComponent(std::move(spriteRenderer));
        pauseRenderer->Initialize();
    }

    {
        auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"lost.dds");
        looseRenderer = spriteRenderer.get();
        AddComponent(std::move(spriteRenderer));
        looseRenderer->Initialize();
    }

    {
        auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"win.dds");
        winRenderer = spriteRenderer.get();
        AddComponent(std::move(spriteRenderer));
        winRenderer->Initialize();
    }

    PM3D_API::EventSystem::Subscribe([this](const GameOverEvent& event)
    {
        std::cout << "GameOverEvent received" << std::endl;

        isPaused = true;
        isEnded = true;
        isWon = event.IsWon();
        PM3D::Time::GetInstance().SetTimeScale(0.f);
    });

    std::cout << "PauseComponent::Initialize()" << std::endl;
}

void PauseComponent::Draw()
{
    if (!isPaused && alpha == 0.f) return;

    if (isEnded)
    {
        if (isWon)
        {
            winRenderer->DrawSelf();
        }
        else
        {
            looseRenderer->DrawSelf();
        }
    }
    else
    {
        pauseRenderer->DrawSelf();
    }
}

void PauseComponent::TogglePause()
{
    if (isPaused)
    {
        isPaused = false;
        PM3D::Time::GetInstance().SetTimeScale(1.f);
    }
    else
    {
        isPaused = true;
        PM3D::Time::GetInstance().SetTimeScale(0.f);
    }

    PM3D_API::EventSystem::Publish(PauseEvent(isPaused));
}

void PauseComponent::Update()
{
    UIObject::Update();

    if (alpha < 0.001f && alpha != 0.f)
    {
        alpha = 0.f;
    }
    else
    {
        alpha = Util::Lerp(alpha, isPaused ? 1.f : 0.f, 20.f * PM3D::Time::GetInstance().GetUpdateDeltaTimeA());
    }

    if (Input::IsKeyPressed(KeyCode::ESCAPE))
    {
        if (isEnded) return;

        TogglePause();
    }

    if (isPaused && Input::IsKeyPressed(KeyCode::R))
    {
        PM3D_API::EventSystem::Publish(RestartEvent());
        isPaused = false;
        isEnded = false;
        isWon = false;
        // PM3D::Time::GetInstance().SetTimeScale(1.f);
    }
}
