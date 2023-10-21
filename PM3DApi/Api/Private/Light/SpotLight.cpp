#include "../../Public/Light/SpotLight.h"

PM3D_API::ShaderLightDefaultParameters PM3D_API::SpotLight::GetShaderLightDefaultParameters() const
{
	return {
		true,
		3, // Spot
		DirectX::XMFLOAT4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f),
		DirectX::XMFLOAT4(worldRotationEuler.x, worldRotationEuler.y, worldRotationEuler.z, 0.0f),

		{0.2f * color.x, 0.2f * color.y, 0.2f * color.z, 1.0f}, // Ambient
		{color.x, color.y, color.z, 1.0f}, // Diffuse
		{0.6f * color.x, 0.6f * color.y, 0.6f * color.z, 1.0f}, // Specular
		
		intensity, // Specular power
		innerAngle,
		outerAngle
	};
}
