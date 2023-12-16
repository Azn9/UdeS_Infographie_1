#pragma once

#include <DirectXMath.h>

namespace PM3D_API
{
    struct ShaderLightDefaultParameters
    {
        DirectX::XMMATRIX matWorldViewProj;

        DirectX::XMVECTOR position;
        DirectX::XMVECTOR direction;

        DirectX::XMVECTOR ambient;
        DirectX::XMVECTOR diffuse;
        DirectX::XMVECTOR specular;
        float specularPower;

        // Only for spot light
        float innerAngle;
        float outerAngle;

        int initialized;
        int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot

        DirectX::XMFLOAT3 padding;
    };

    static ShaderLightDefaultParameters DefaultShaderLightDefaultParameters = {
        DirectX::XMMatrixIdentity(),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        0.0f,
        0.0f,
        0.0f,
        false,
        0,
        {0.0f, 0.0f, 0.0f}
    };
}
