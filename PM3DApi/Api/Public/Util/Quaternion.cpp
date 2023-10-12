#include "Quaternion.h"

#include <cmath>

DirectX::XMFLOAT3 Quaternion::ToEulerAngles() const
{
	DirectX::XMVECTOR vec;
	float angle;

	DirectX::XMQuaternionToAxisAngle(
		&vec,
		&angle,
		XMLoadFloat4(this)
	);

	DirectX::XMFLOAT3 euler;

	const float s = std::sin(angle);
	const float c = std::cos(angle);
	const float t = 1.0f - c;

	const float vecX = DirectX::XMVectorGetX(vec);
	const float vecY = DirectX::XMVectorGetY(vec);
	const float vecZ = DirectX::XMVectorGetZ(vec);

	euler.z = atan2f(vecX * vecY * t + vecZ * s, 1.0f - (vecX * vecX + vecZ * vecZ) * t);
	euler.x = asinf(-2.0f * (vecX * vecZ - vecY * s));
	euler.y = atan2f(vecY * vecZ * t + vecX * s, 1.0f - (vecY * vecY + vecZ * vecZ) * t);

	return euler;
}

Quaternion&& Quaternion::operator*(const Quaternion& quaternion) const
{
	const auto result = DirectX::XMQuaternionMultiply(XMLoadFloat4(this), XMLoadFloat4(&quaternion));

	XMFLOAT4 resultVector;
	XMStoreFloat4(&resultVector, result);
	
	return Quaternion(resultVector);
}

DirectX::XMFLOAT4 Quaternion::EulerToQuaternion(const DirectX::XMFLOAT3 euler)
{
	XMFLOAT4 quaternion;
	XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&euler)));
	return quaternion;
}
