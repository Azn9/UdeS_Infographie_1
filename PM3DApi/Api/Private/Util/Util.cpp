#include "../../Public/Util/Util.h"

DirectX::XMFLOAT3 Util::Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t)
{
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	};
}

DirectX::XMFLOAT2 Util::operator*(const DirectX::XMFLOAT2& a, const float& f)
{
	return {a.x * f, a.y * f};
}

DirectX::XMFLOAT2 Util::operator/(const DirectX::XMFLOAT2& a, const float& f)
{
	return {a.x / f, a.y / f};
}

DirectX::XMFLOAT2 Util::operator+(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return {a.x + b.x, a.y + b.y};
}

DirectX::XMFLOAT2 Util::operator-(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return {a.x - b.x, a.y - b.y};
}
