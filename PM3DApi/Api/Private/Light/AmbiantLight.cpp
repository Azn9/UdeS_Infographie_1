#include "Api/Public/Light/AmbiantLight.h"

PM3D_API::ShaderLightDefaultParameters PM3D_API::AmbiantLight::GetShaderLightDefaultParameters(
    GameObject* gameObject) const
{
    return {
        DirectX::XMMatrixIdentity(),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        DirectX::XMVectorSet(color.x, color.y, color.z, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        0.0f,
        0.0f,
        0.0f,
        true,
        0,
        {0.0f, 0.0f, 0.0f}
    };
}

PM3D_API::ShaderLightDefaultParameters PM3D_API::AmbiantLight::GetShaderLightDefaultParameters(
    DirectX::XMMATRIX wvp) const
{
    return {
        wvp,
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        DirectX::XMVectorSet(color.x, color.y, color.z, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        0.0f,
        0.0f,
        0.0f,
        true,
        0,
        {0.0f, 0.0f, 0.0f}
    };
}
