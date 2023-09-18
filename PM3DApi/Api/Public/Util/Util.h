#pragma once
#include <DirectXMath.h>

class Util
{
public:
	static DirectX::XMFLOAT3 Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t);
	static DirectX::XMFLOAT3 QuaternionToEuler(DirectX::XMFLOAT4 xmfloat4);

};
