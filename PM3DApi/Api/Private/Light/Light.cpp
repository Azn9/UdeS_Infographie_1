#include "../../Public/Light/Light.h"

Light::Light(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation
) : GameObject(
		"Light",
		position,
		rotation,
		{0, 0, 0}
	),
    intensity(1.0f),
    color({1.0f, 1.0f, 1.0f})
{

}

Light::Light(
	const std::string& name,
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation
) : GameObject(
		name,
		position,
		rotation,
		{0, 0, 0}
	),
	intensity(1.0f),
	color({1.0f, 1.0f, 1.0f})
{

}

Light::Light(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const float intensity
) : GameObject(
		"Light",
		position,
		rotation,
		{0, 0, 0}
	),
    intensity(intensity),
    color({1.0f, 1.0f, 1.0f})
{

}

Light::Light(
	const std::string& name,
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const float intensity
) : GameObject(
		name,
		position,
		rotation,
		{0, 0, 0}
	),
	intensity(intensity),
	color({1.0f, 1.0f, 1.0f})
{

}

Light::Light(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const float intensity,
	const DirectX::XMFLOAT3 color
) : GameObject(
		"Light",
		position,
		rotation,
		{0, 0, 0}
	),
    intensity(intensity),
    color(color)
{

}

Light::Light(
	const std::string& name,
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const float intensity,
	const DirectX::XMFLOAT3 color
) : GameObject(
		name,
		position,
		rotation,
		{0, 0, 0}
	),
	intensity(intensity),
	color(color)
{

}
