#pragma once

#include "../../Public/Util/Util.h"


namespace PM3D_API
{
using namespace Util;

class Camera;
    
class Frustrum
{
public:

    void SetPlanes(const Camera& cam);

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
