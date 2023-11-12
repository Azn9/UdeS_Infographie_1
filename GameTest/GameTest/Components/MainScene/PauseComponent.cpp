#include "PauseComponent.h"

#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/Input/Input.h"
#include "Api/Public/Util/Util.h"
#include "Core/Public/Util/Time.h"
#include "GameTest/RestartEvent.h"

void PauseComponent::Initialize()
{
    UIObject::Initialize();
    
    auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"pause.dds");
    const auto spriteRendererPtr = spriteRenderer.get();
    AddComponent(std::move(spriteRenderer));
    spriteRendererPtr->Initialize();

    std::cout << "PauseComponent::Initialize()" << std::endl;
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
        std::cout << "PauseComponent::Update() : ESCAPE pressed" << std::endl;
        
        if (isPaused)
        {
            isPaused = false;
            PM3D::Time::GetInstance().SetTimeScale(1.f);
        } else
        {
            isPaused = true;
            PM3D::Time::GetInstance().SetTimeScale(0.f);
        }

        std::cout << "PauseComponent::Update() : isPaused = " << isPaused << std::endl;
    }

    if (isPaused && Input::IsKeyPressed(KeyCode::R))
    {
        // TODO : restart
        PM3D_API::EventSystem::Publish(RestartEvent());
        isPaused = false;
        
    }
}
