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
	std::string ws2s(const std::wstring& wstr);
};

