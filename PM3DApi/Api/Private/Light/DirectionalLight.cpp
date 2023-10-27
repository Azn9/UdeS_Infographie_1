#include "../../Public/Light/DirectionalLight.h"

PM3D_API::DirectionalLight::DirectionalLight(
	const DirectX::XMFLOAT3 rotation
) : Light({0, 0, 0}, rotation)
{

}

PM3D_API::DirectionalLight::DirectionalLight(
	const DirectX::XMFLOAT3 rotation,
	const float intensity
) : Light({0, 0, 0}, rotation, intensity)
{

}

PM3D_API::DirectionalLight::DirectionalLight(
	const DirectX::XMFLOAT3 rotation,
	const float intensity,
	const DirectX::XMFLOAT3 color
) : Light({0, 0, 0}, rotation, intensity, color)
{

}

PM3D_API::DirectionalLight::DirectionalLight(
	const std::string& name,
	const DirectX::XMFLOAT3 rotation
) : Light(name, {0, 0, 0}, rotation)
{

}

PM3D_API::DirectionalLight::DirectionalLight(
	const std::string& name,
	const DirectX::XMFLOAT3 rotation,
	const float intensity
) : Light(name, {0, 0, 0}, rotation, intensity)
{

}

PM3D_API::DirectionalLight::DirectionalLight(
	const std::string& name,
	const DirectX::XMFLOAT3 rotation,
	const float intensity,
	const DirectX::XMFLOAT3 color
) : Light(name, {0, 0, 0}, rotation, intensity, color)
{

}

PM3D_API::ShaderLightDefaultParameters PM3D_API::DirectionalLight::GetShaderLightDefaultParameters(GameObject* gameObject) const
{
	const auto mVLight = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f),
		DirectX::XMVectorSet(gameObject->GetWorldPosition().x, gameObject->GetWorldPosition().y,
							 gameObject->GetWorldPosition().z, 1.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
	);

	constexpr float nearPlane = 2.0;
	constexpr float farPlane = 100.0;

	const auto mPLight = DirectX::XMMatrixOrthographicLH(
		512,
		512,
		nearPlane,
		farPlane
	);

	const auto matWorldViewProj = mVLight * mPLight;
	
	return std::move(ShaderLightDefaultParameters{
		true,
		1, // Directional
		{0.0f, 0.0f, 0.0f, 1.0f}, // Position
		DirectX::XMFLOAT4(worldRotationEuler.x, worldRotationEuler.y, worldRotationEuler.z, 0.0f),

		{0.2f * color.x, 0.2f * color.y, 0.2f * color.z, 1.0f}, // Ambient
		{color.x, color.y, color.z, 1.0f}, // Diffuse
		{0.6f * color.x, 0.6f * color.y, 0.6f * color.z, 1.0f}, // Specular
		
		intensity, // Specular power

		// Unused here
		0.0f, // Inner angle
		0.0f, // Outer angle

		matWorldViewProj
	});
}
