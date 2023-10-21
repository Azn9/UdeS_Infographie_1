#include "../../Public/Light/PointLight.h"

PM3D_API::ShaderLightDefaultParameters PM3D_API::PointLight::GetShaderLightDefaultParameters() const
{
	return {
		true,
		static_cast<int>(LightType::POINT),
		DirectX::XMFLOAT4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f),
		{0.0f, 0.0f, 0.0f, 0.0f},

		{0.2f * color.x, 0.2f * color.y, 0.2f * color.z, 1.0f},
		{color.x, color.y, color.z, 1.0f},
		{0.6f * color.x, 0.6f * color.y, 0.6f * color.z, 1.0f},

		intensity,

		0.0f,
		0.0f
	};
}
