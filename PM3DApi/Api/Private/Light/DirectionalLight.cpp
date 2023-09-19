#include "Api/Public/Light/DirectionalLight.h"

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
