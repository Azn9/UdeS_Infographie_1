#include "Api/Public/Light/Light.h"

Light::Light(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation
) : Component(position, rotation, {0, 0, 0}),
    Intensity(1.0f),
    Color({1.0f, 1.0f, 1.0f})
{

}

Light::Light(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const float intensity
) : Component(position, rotation, {0, 0, 0}),
    Intensity(intensity),
    Color({1.0f, 1.0f, 1.0f})
{

}

Light::Light(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const float intensity,
	const DirectX::XMFLOAT3 color
) : Component(position, rotation, {0, 0, 0}),
    Intensity(intensity),
    Color(color)
{

}
