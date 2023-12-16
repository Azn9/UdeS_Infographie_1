#include "Api/Public/Light/Light.h"

#include "Api/Public/Scene/Scene.h"

PM3D_API::Light::Light(
    const DirectX::XMFLOAT3 position,
    const DirectX::XMFLOAT3 rotation
) : GameObject(
        "Light",
        position,
        rotation,
        {0, 0, 0}
    ),
    intensity(1.0f),
    color({1.0f, 1.0f, 1.0f})
{
}

PM3D_API::Light::Light(
    const std::string& name,
    const DirectX::XMFLOAT3 position,
    const DirectX::XMFLOAT3 rotation
) : GameObject(
        name,
        position,
        rotation,
        {0, 0, 0}
    ),
    intensity(1.0f),
    color({1.0f, 1.0f, 1.0f})
{
}

PM3D_API::Light::Light(
    const DirectX::XMFLOAT3 position,
    const DirectX::XMFLOAT3 rotation,
    const float intensity
) : GameObject(
        "Light",
        position,
        rotation,
        {0, 0, 0}
    ),
    intensity(intensity),
    color({1.0f, 1.0f, 1.0f})
{
}

PM3D_API::Light::Light(
    const std::string& name,
    const DirectX::XMFLOAT3 position,
    const DirectX::XMFLOAT3 rotation,
    const float intensity
) : GameObject(
        name,
        position,
        rotation,
        {0, 0, 0}
    ),
    intensity(intensity),
    color({1.0f, 1.0f, 1.0f})
{
}

PM3D_API::Light::Light(
    const DirectX::XMFLOAT3 position,
    const DirectX::XMFLOAT3 rotation,
    const float intensity,
    const DirectX::XMFLOAT3 color
) : GameObject(
        "Light",
        position,
        rotation,
        {0, 0, 0}
    ),
    intensity(intensity),
    color(color)
{
}

PM3D_API::Light::Light(
    const std::string& name,
    const DirectX::XMFLOAT3 position,
    const DirectX::XMFLOAT3 rotation,
    const float intensity,
    const DirectX::XMFLOAT3 color
) : GameObject(
        name,
        position,
        rotation,
        {0, 0, 0}
    ),
    intensity(intensity),
    color(color)
{
}

void PM3D_API::Light::SetLocalPosition(const DirectX::XMFLOAT3 newPosition)
{
    GameObject::SetLocalPosition(newPosition);
    scene->SetLightsNeedUpdate(true);
}

void PM3D_API::Light::SetWorldPosition(const DirectX::XMFLOAT3 newPosition)
{
    GameObject::SetWorldPosition(newPosition);
    scene->SetLightsNeedUpdate(true);
}

void PM3D_API::Light::SetLocalRotation(const Quaternion newRotation)
{
    GameObject::SetLocalRotation(newRotation);
    scene->SetLightsNeedUpdate(true);
}

void PM3D_API::Light::SetWorldRotation(const Quaternion newRotation)
{
    GameObject::SetWorldRotation(newRotation);
    scene->SetLightsNeedUpdate(true);
}

void PM3D_API::Light::SetLocalRotation(const DirectX::XMFLOAT3 newRotation)
{
    GameObject::SetLocalRotation(newRotation);
    scene->SetLightsNeedUpdate(true);
}

void PM3D_API::Light::SetWorldRotation(const DirectX::XMFLOAT3 newRotation)
{
    GameObject::SetWorldRotation(newRotation);
    scene->SetLightsNeedUpdate(true);
}
