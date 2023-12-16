#pragma once

#include <DirectXMath.h>

namespace PM3D_API
{
    class Plane
    {
    public:
        Plane() = default;
        Plane(const DirectX::XMVECTOR& normal, const DirectX::XMVECTOR& point);
        float SignedDistanceFromPlane(const DirectX::XMVECTOR& point) const;
        bool IsSphereInFrontOfPlane(const DirectX::XMVECTOR& point, const float& radius) const;

        DirectX::XMVECTOR GetNormal() const { return normal; }
        float GetDist() const { return dist; }

    private:
        DirectX::XMVECTOR normal;
        float dist;
    };
}
