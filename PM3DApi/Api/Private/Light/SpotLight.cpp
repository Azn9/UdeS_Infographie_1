#include "Api/Public/Light/SpotLight.h"

PM3D_API::ShaderLightDefaultParameters PM3D_API::SpotLight::GetShaderLightDefaultParameters(
    GameObject* gameObject) const
{
    DirectX::XMMATRIX matWorldViewProj;

    if (gameObject == nullptr)
    {
        matWorldViewProj = DirectX::XMMatrixIdentity();
    }
    else
    {
        const auto mVLight = DirectX::XMMatrixLookAtRH(
            DirectX::XMVectorSet(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f),
            DirectX::XMVectorSet(gameObject->GetWorldPosition().x, gameObject->GetWorldPosition().y,
                                 gameObject->GetWorldPosition().z, 1.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
        );

        constexpr float fov = DirectX::XM_PI / 4;
        constexpr float aspectRatio = 1.0f;
        constexpr float nearPlane = 2.0;
        constexpr float farPlane = 100.0;

        const auto mPLight = DirectX::XMMatrixPerspectiveFovRH(
            fov,
            aspectRatio,
            nearPlane,
            farPlane
        );

        matWorldViewProj = mVLight * mPLight;
    }

    return {
        matWorldViewProj,
        DirectX::XMVectorSet(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f),
        DirectX::XMVectorSet(GetWorldDirection().x, GetWorldDirection().y, GetWorldDirection().z, 0.0f),

        DirectX::XMVectorSet(0.2f * color.x, 0.2f * color.y, 0.2f * color.z, 1.0f), // Ambient
        DirectX::XMVectorSet(color.x, color.y, color.z, 1.0f), // Diffuse
        DirectX::XMVectorSet(0.6f * color.x, 0.6f * color.y, 0.6f * color.z, 1.0f), // Specular

        intensity, // Specular power
        innerAngle,
        outerAngle,

        true,
        3,
        {0.0f, 0.0f, 0.0f}
    };
}

PM3D_API::ShaderLightDefaultParameters PM3D_API::SpotLight::GetShaderLightDefaultParameters(DirectX::XMMATRIX wvp) const
{
    return {
        wvp,
        DirectX::XMVectorSet(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f),
        DirectX::XMVectorSet(GetWorldDirection().x, GetWorldDirection().y, GetWorldDirection().z, 0.0f),

        DirectX::XMVectorSet(0.2f * color.x, 0.2f * color.y, 0.2f * color.z, 1.0f), // Ambient
        DirectX::XMVectorSet(color.x, color.y, color.z, 1.0f), // Diffuse
        DirectX::XMVectorSet(0.6f * color.x, 0.6f * color.y, 0.6f * color.z, 1.0f), // Specular

        intensity, // Specular power
        innerAngle,
        outerAngle,

        true,
        3,
        {0.0f, 0.0f, 0.0f}
    };
}
