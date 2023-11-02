#pragma once

#include "Api/Public/Camera/Camera.h"
#include "Api/Public/Util/Util.h"


namespace PM3D_API
{
using namespace Util;
    
class Frustrum
{
public:

    void SetPlanes(const PM3D_API::Camera&);

private:
    Plane nearPlane;
    Plane farPlane;
    Plane leftPlane;
    Plane rightPlane;
    Plane topPlane;
    Plane bottomPlane;

    
};
}
