#include "Api/Public/Util/Quaternion.h"

#include <algorithm>
#include <cmath>

DirectX::XMFLOAT3 PM3D_API::Quaternion::ToEulerAngles() const
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

PM3D_API::Quaternion PM3D_API::Quaternion::operator*(const PM3D_API::Quaternion& quaternion) const
{
    const auto result = DirectX::XMQuaternionMultiply(XMLoadFloat4(this), XMLoadFloat4(&quaternion));

    XMFLOAT4 resultVector;
    XMStoreFloat4(&resultVector, result);

    return PM3D_API::Quaternion(resultVector);
    // TODO Warning C4172 : retourne l'adresse d'une variable locale ou  temporaire
}

DirectX::XMVECTOR PM3D_API::Quaternion::ToXMVector() const
{
    return XMLoadFloat4(this);
}

DirectX::XMFLOAT4 PM3D_API::Quaternion::EulerToQuaternion(const DirectX::XMFLOAT3 euler)
{
    XMFLOAT4 quaternion;
    const DirectX::XMFLOAT3 radRotation = {
        euler.x * (DirectX::XM_PI / 180.0f),
        euler.y * (DirectX::XM_PI / 180.0f),
        euler.z * (DirectX::XM_PI / 180.0f)
    };

    XMStoreFloat4(&quaternion,
                  DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&radRotation)));
    return quaternion;
}
