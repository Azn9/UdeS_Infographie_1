#pragma once

#include <DirectXMath.h>
#include <string>

#include "Quaternion.h"

namespace Util
{
	DirectX::XMFLOAT3 Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t);
	DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
	DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
	DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& a, const float& f);
	DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& a, const float& f);
	DirectX::XMFLOAT3 Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t);
	DirectX::XMFLOAT3 DirectionToEulerAngles(DirectX::XMFLOAT3 direction);
	PM3D_API::Quaternion DirectionToQuaternion(DirectX::XMFLOAT3 direction);

	float magnitude(const DirectX::XMFLOAT2& f2);
	float magnitude(const DirectX::XMFLOAT3& f3);

	
	const DirectX::XMVECTOR VECTORFORWARD = DirectX::XMVECTOR{0.f,0.f,1.f,0.f};
	const DirectX::XMVECTOR VECTORRIGHT = DirectX::XMVECTOR{-1.f,0.f,0.f,0.f};
	const DirectX::XMVECTOR VECTORUP = DirectX::XMVECTOR{0.f,1.f,0.f,0.f};
	const DirectX::XMVECTOR VECTORZERO = DirectX::XMVECTOR{0.f,0.f,0.f,0.f};


	void DrawDebugVector3(const std::string name, const DirectX::XMVECTOR& vec);
};

