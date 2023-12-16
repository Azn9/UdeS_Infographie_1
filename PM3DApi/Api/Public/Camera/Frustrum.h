#pragma once

#include "Api/Public/Util/Util.h"
#include "Api/Public/Util/Plane.h"


namespace PM3D_API
{
    using namespace Util;

    class Camera;

    class Frustrum
    {
    public:
        void SetPlanes(const Camera& cam);

        /// \brief checks if a sphere at point of radius radius is intersacting with the frustrum
        /// \param point Must be in view space
        /// \param radius Must be in view space
        /// \return 
        bool ContainsSphere(const DirectX::XMVECTOR& point, const float& radius = 0.f) const;

        void DrawDebugInfo() const;

    private:
        Plane nearPlane;
        Plane farPlane;
        Plane leftPlane;
        Plane rightPlane;
        Plane topPlane;
        Plane bottomPlane;
    };
}
