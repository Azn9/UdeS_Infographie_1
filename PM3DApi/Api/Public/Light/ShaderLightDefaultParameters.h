#pragma once

#include <DirectXMath.h>

struct ShaderLightDefaultParameters
{
    int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot
    
    DirectX::XMVECTOR position;
    DirectX::XMVECTOR direction;
    
    DirectX::XMFLOAT3 ambient;
    DirectX::XMFLOAT3 diffuse;
    DirectX::XMFLOAT3 specular;
    
    DirectX::XMFLOAT3 color;
    
    // Only for spot light
    float innerAngle;
    float outerAngle;
};
