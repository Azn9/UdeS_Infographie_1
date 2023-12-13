#include "FadeWhiteComponent.h"

#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Core/Public/Util/Time.h"

void FadeWhiteComponent::Initialize()
{
    auto whiteRender = std::make_unique<PM3D_API::SpriteRenderer>(L"sprite/ui/white.dds");
    const auto whiteRenderPtr = whiteRender.get();
    AddComponent(std::move(whiteRender));
    whiteRenderPtr->Initialize();
}

void FadeWhiteComponent::Update()
{
    UIObject::Update();

    if (fading)
    {
        alpha = Util::Lerp(alpha, toTransparent ? 0 : 1, speed * PM3D::Time::GetInstance().GetUpdateDeltaTime());

        if (toTransparent && alpha <= 0.01f)
        {
            alpha = 0;
            fading = false;
        }
        else if (!toTransparent && alpha >= 0.99f)
        {
            alpha = 1;
            fading = false;
        }
    }
}
