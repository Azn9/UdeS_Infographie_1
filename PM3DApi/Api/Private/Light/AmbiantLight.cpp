#include "../../Public/Light/AmbiantLight.h"

PM3D_API::ShaderLightDefaultParameters PM3D_API::AmbiantLight::GetShaderLightDefaultParameters() const
{
	return {
		true,
		0,
		{0.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		DirectX::XMFLOAT4(color.x, color.y, color.z, 1.0f),
		{0.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
		0.0f,
		0.0f,
		0.0f
	};
}
