#pragma once
#include <DirectXMath.h>

class Quaternion : public DirectX::XMFLOAT4
{
public:
	Quaternion() : XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) {}
	
	explicit Quaternion(const DirectX::XMFLOAT3& fromEulerAngles) : XMFLOAT4(EulerToQuaternion(fromEulerAngles)) {};
	explicit Quaternion(const XMFLOAT4& quaternion) : XMFLOAT4(quaternion) {}

	DirectX::XMFLOAT3 ToEulerAngles() const;

	Quaternion&& operator*(const Quaternion& quaternion) const;
	DirectX::XMVECTOR ToXMVector() const;

	static Quaternion FromEulerAngles(const DirectX::XMFLOAT3& euler) { return Quaternion(EulerToQuaternion(euler)); }
private:
	static XMFLOAT4 EulerToQuaternion(DirectX::XMFLOAT3 euler);
};
