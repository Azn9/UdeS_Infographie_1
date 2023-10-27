#include "../../Public/Light/AmbiantLight.h"

PM3D_API::ShaderLightDefaultParameters PM3D_API::AmbiantLight::GetShaderLightDefaultParameters(GameObject* gameObject) const
{
	return {
		true,
		0,
		DirectX::XMFLOAT2(0, 0),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(color.x, color.y, color.z, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		0.0f,
		0.0f,
		0.0f,
		DirectX::XMMatrixIdentity()
	};
}
