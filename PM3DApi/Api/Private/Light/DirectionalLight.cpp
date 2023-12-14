#include "Api/Public/Light/DirectionalLight.h"
#include "Api/Private/Light/Shadow/ShadowProcessor.h"
#include <iostream>

#include "Api/Public/Util/Util.h"

PM3D_API::DirectionalLight::DirectionalLight(
    const DirectX::XMFLOAT3 direction
) : Light({0, 0, 0}, Util::DirectionToEulerAngles(direction)), direction(direction)
{
}

PM3D_API::DirectionalLight::DirectionalLight(
    const DirectX::XMFLOAT3 direction,
    const float intensity
) : Light({0, 0, 0}, Util::DirectionToEulerAngles(direction), intensity), direction(direction)
{
}

PM3D_API::DirectionalLight::DirectionalLight(
    const DirectX::XMFLOAT3 direction,
    const float intensity,
    const DirectX::XMFLOAT3 color
) : Light({0, 0, 0}, Util::DirectionToEulerAngles(direction), intensity, color), direction(direction)
{
}

PM3D_API::DirectionalLight::DirectionalLight(
    const std::string& name,
    const DirectX::XMFLOAT3 direction
) : Light(name, {0, 0, 0}, Util::DirectionToEulerAngles(direction)), direction(direction)
{
}

PM3D_API::DirectionalLight::DirectionalLight(
    const std::string& name,
    const DirectX::XMFLOAT3 direction,
    const float intensity
) : Light(name, {0, 0, 0}, Util::DirectionToEulerAngles(direction), intensity), direction(direction)
{
}

PM3D_API::DirectionalLight::DirectionalLight(
    const std::string& name,
    const DirectX::XMFLOAT3 direction,
    const float intensity,
    const DirectX::XMFLOAT3 color
) : Light(name, {0, 0, 0}, Util::DirectionToEulerAngles(direction), intensity, color), direction(direction)
{
}

void PM3D_API::DirectionalLight::SetDirection(const DirectX::XMFLOAT3 newDirection)
{
    direction = newDirection;
    SetLocalRotation(Util::DirectionToQuaternion(direction));
}

PM3D_API::ShaderLightDefaultParameters PM3D_API::DirectionalLight::GetShaderLightDefaultParameters(GameObject* go) const
{
    const auto worldPosition = GetWorldPosition();
    const auto worldDirection = GetWorldDirection();

    const auto position = DirectX::XMVectorSet(
        worldPosition.x - 10 * worldDirection.x,
        worldPosition.y - 10 * worldDirection.y,
        worldPosition.z - 10 * worldDirection.z,
        1.0f
    );
    const auto focusPoint = DirectX::XMVectorSet(
                worldPosition.x,
                worldPosition.y,
                worldPosition.z,
                1.0f
            );
    const auto mVLight = DirectX::XMMatrixLookAtRH(
        position,
        focusPoint,
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );

    constexpr float nearPlane = 0.02f;
    constexpr float farPlane = 500.0f;
    
    const auto mPLight = DirectX::XMMatrixOrthographicRH(
        ShadowProcessor::viewWidthDirectionnalLight,
        ShadowProcessor::viewHeightDirectionnalLight,
        nearPlane,
        farPlane
    );

    const DirectX::XMMATRIX vp = mVLight * mPLight;

    return std::move(ShaderLightDefaultParameters{
        DirectX::XMMatrixTranspose(go->GetMatWorld() * vp),

        DirectX::XMVectorSet(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f),
        DirectX::XMVectorSet(GetWorldDirection().x, GetWorldDirection().y, GetWorldDirection().z, 0.0f),

        DirectX::XMVectorSet(0.2f * color.x, 0.2f * color.y, 0.2f * color.z, 1.0f), // Ambient
        DirectX::XMVectorSet(color.x, color.y, color.z, 1.0f), // Diffuse
        DirectX::XMVectorSet(0.6f * color.x, 0.6f * color.y, 0.6f * color.z, 1.0f), // Specular

        intensity, // Specular power

        // Unused here
        0.0f, // Inner angle
        0.0f, // Outer angle

        true,
        1, // Directional

        {0.0f, 0.0f, 0.0f}
    });
}

PM3D_API::ShaderLightDefaultParameters PM3D_API::DirectionalLight::GetShaderLightDefaultParameters(
    DirectX::XMMATRIX wvp) const
{
    return std::move(ShaderLightDefaultParameters{
        wvp,
        
        DirectX::XMVectorSet(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z, 1.0f), // Position
        DirectX::XMVectorSet(GetWorldDirection().x, GetWorldDirection().y, GetWorldDirection().z, 0.0f),

        DirectX::XMVectorSet(0.2f * color.x, 0.2f * color.y, 0.2f * color.z, 1.0f), // Ambient
        DirectX::XMVectorSet(color.x, color.y, color.z, 1.0f), // Diffuse
        DirectX::XMVectorSet(0.6f * color.x, 0.6f * color.y, 0.6f * color.z, 1.0f), // Specular

        intensity, // Specular power

        // Unused here
        0.0f, // Inner angle
        0.0f, // Outer angle

        true,
        1, // Directional

        {0.0f, 0.0f, 0.0f}
    });
}
