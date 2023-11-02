#pragma once

#include <DirectXMath.h>
#include <string>

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
		DirectX::XMVECTOR normal;
		DirectX::XMVECTOR point;
	};

	DirectX::XMVECTOR operator+(const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b);
	DirectX::XMVECTOR operator-(const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b);
	DirectX::XMVECTOR operator-(const DirectX::XMVECTOR& a);
	// /!\ FOR VECTOR3
	DirectX::XMVECTOR Float3ToVec(const DirectX::XMFLOAT3& f3);
	DirectX::XMVECTOR operator*(const DirectX::XMVECTOR& a, const float& f);
	DirectX::XMVECTOR operator/(const DirectX::XMVECTOR& a, const float& f);

	void DrawDebugVector3(const std::string name, const DirectX::XMVECTOR& vec);
};

