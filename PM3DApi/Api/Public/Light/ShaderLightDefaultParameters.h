#pragma once

#include <DirectXMath.h>

namespace PM3D_API
{
struct ShaderLightDefaultParameters
{
	int initialized;
	int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot
	
	DirectX::XMFLOAT2 padding2;
	
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR direction;

	DirectX::XMVECTOR ambient;
	DirectX::XMVECTOR diffuse;
	DirectX::XMVECTOR specular;
	float specularPower;

	// Only for spot light
	float innerAngle;
	float outerAngle;

	DirectX::XMMATRIX matWorldViewProj;
};

static ShaderLightDefaultParameters DefaultShaderLightDefaultParameters = {
	false,
	0,
	DirectX::XMFLOAT2(0, 0),
	DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
	DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
	DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
	DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
	DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
	0.0f,
	0.0f,
	0.0f,
	DirectX::XMMatrixIdentity()
};
}
