#include "../../Public/Light/DirectionalLight.h"

DirectionalLight::DirectionalLight(
	const DirectX::XMFLOAT3 rotation
) : Light({0, 0, 0}, rotation)
{

}

DirectionalLight::DirectionalLight(
	const DirectX::XMFLOAT3 rotation,
	const float intensity
) : Light({0, 0, 0}, rotation, intensity)
{

}

DirectionalLight::DirectionalLight(
	const DirectX::XMFLOAT3 rotation,
	const float intensity,
	const DirectX::XMFLOAT3 color
) : Light({0, 0, 0}, rotation, intensity, color)
{

}

DirectionalLight::DirectionalLight(
	const std::string& name,
	const DirectX::XMFLOAT3 rotation
) : Light(name, {0, 0, 0}, rotation)
{

}

DirectionalLight::DirectionalLight(
	const std::string& name,
	const DirectX::XMFLOAT3 rotation,
	const float intensity
) : Light(name, {0, 0, 0}, rotation, intensity)
{

}

DirectionalLight::DirectionalLight(
	const std::string& name,
	const DirectX::XMFLOAT3 rotation,
	const float intensity,
	const DirectX::XMFLOAT3 color
) : Light(name, {0, 0, 0}, rotation, intensity, color)
{

}

ShaderLightDefaultParameters DirectionalLight::GetShaderLightDefaultParameters() const
{
	return {
		1, // Directional
		{0.0f, 0.0f, 0.0f, 1.0f}, // Position
		DirectX::XMVectorSet(worldRotationEuler.x, worldRotationEuler.y, worldRotationEuler.z, 1.0f),

		{0.2f, 0.2f, 0.2f}, // Ambient
		{1.0f, 1.0f, 1.0f}, // Diffuse
		{0.6f, 0.6f, 0.6f}, // Specular
		
		color, // Color

		// Unused here
		0.0f, // Inner angle
		0.0f // Outer angle
	};
}
