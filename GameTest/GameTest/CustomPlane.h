#pragma once

#include "Components/SnowRenderer.h"
#include "Shader/SnowShader.h"

class CustomPlane final : public PM3D_API::GameObject
{
public:
    CustomPlane() : GameObject("TestPlane") {}
    void Initialize() override
    {
        auto shader = std::make_unique<SnowShader>(L"SnowShader.fx");
        auto renderer = std::make_unique<SnowRenderer>(std::move(shader), "snowplane.obj");
        renderer->Initialize();
        AddComponent(std::move(renderer));
    }
};
