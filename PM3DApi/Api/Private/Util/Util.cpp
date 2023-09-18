#include "Api/Public/Util/Util.h"

DirectX::XMFLOAT3 Util::Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t)
{
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	};
}

DirectX::XMFLOAT3 Util::QuaternionToEuler(DirectX::XMFLOAT4 xmfloat4)
{
	DirectX::XMFLOAT3 euler;
	XMStoreFloat3(&euler, DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat4(&xmfloat4)));
	return euler;
}
