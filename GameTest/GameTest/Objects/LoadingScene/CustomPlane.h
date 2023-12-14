#pragma once

#include "GameTest/Components/SnowRenderer.h"
#include "GameTest/Shader/SnowShader.h"

class CustomPlane final : public PM3D_API::GameObject
{
public:
    CustomPlane() : GameObject("TestPlane")
    {
    }

    void Initialize() override
    {
        auto shader = std::make_unique<SnowShader>(L"shader/SnowShader.fx");
        auto renderer = std::make_unique<SnowRenderer>(std::move(shader), "LoadingScene/ground.obj");
        renderer->Initialize();
        AddComponent(std::move(renderer));
    }
};
