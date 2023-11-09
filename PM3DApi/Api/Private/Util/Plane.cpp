#include "Api/Public/Util/Plane.h"

namespace PM3D_API
{

Plane::Plane(const DirectX::XMVECTOR& _normal, const DirectX::XMVECTOR& point)
    : normal(DirectX::XMVector3Normalize(_normal))
{
    dist = DirectX::XMVector3Dot(point, normal).m128_f32[0];
}

float Plane::SignedDistanceFromPlane(const DirectX::XMVECTOR& point) const
{
    return DirectX::XMVector3Dot(point, normal).m128_f32[0] - dist;
}

bool Plane::IsSphereInFrontOfPlane(const DirectX::XMVECTOR& point, const float& radius) const
{
    float result = SignedDistanceFromPlane(point);
    return  result > radius + 0.01f;
}

}
