#pragma once

#include <DirectXMath.h>

namespace Util
{
	DirectX::XMFLOAT3 Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t);
	DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
	DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
	DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& a, const float& f);
	DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& a, const float& f);
	float magnitude(const DirectX::XMFLOAT2& f2);
	float magnitude(const DirectX::XMFLOAT3& f3);

	struct Plane
	{
		Plane(const DirectX::XMVECTOR& normal, const float& dist) : normal(normal), distance(distance) {}
		
		DirectX::XMVECTOR normal;
		float distance;
	};
};

