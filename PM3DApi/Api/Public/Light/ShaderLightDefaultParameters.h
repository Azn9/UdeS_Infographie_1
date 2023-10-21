#pragma once

#include <DirectXMath.h>

namespace PM3D_API
{
    struct ShaderLightDefaultParameters
    {
        bool initialized;
        int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot
    
        DirectX::XMFLOAT4 position;
        DirectX::XMFLOAT4 direction;
    
        DirectX::XMFLOAT4 ambient;
        DirectX::XMFLOAT4 diffuse;
        DirectX::XMFLOAT4 specular;
        float specularPower;
    
        // Only for spot light
        float innerAngle;
        float outerAngle;
    };
}