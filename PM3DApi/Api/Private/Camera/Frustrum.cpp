#include "Api//Public/Camera/Frustrum.h"
#include "Api/Public/GameHost.h"
#include "Api/Public/Util/Util.h"

using namespace Util;

void PM3D_API::Frustrum::SetPlanes(const PM3D_API::Camera& cam)
{
    const float halfVSide = cam.getFarDist() * tanf(cam.getFieldOfView() * .5f);
    const float halfHSide = halfVSide * PM3D_API::GameHost::GetInstance()->GetAspectRatio();
    const DirectX::XMVECTOR frontMultFar = DirectX::XMVectorScale(cam.GetForwardVector(), cam.getFarDist());

    const DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&cam.GetWorldPosition());
    
    nearPlane = { cam.GetForwardVector(), camPos + cam.GetForwardVector() * cam.getNearDist()};
    
    farPlane = { -cam.GetForwardVector(), camPos + frontMultFar };
    
    rightPlane = { camPos, DirectX::XMVector2Cross(frontMultFar - cam.GetRightVector() * halfHSide, cam.GetUpVector()) };
    
    leftPlane = { camPos, DirectX::XMVector2Cross(cam.GetUpVector(),frontMultFar + cam.GetRightVector() * halfHSide) };
    
    topPlane = { camPos,  DirectX::XMVector2Cross(cam.GetRightVector(), frontMultFar - cam.GetUpVector() * halfVSide) };
    
    bottomPlane = { camPos, DirectX::XMVector2Cross(frontMultFar + cam.GetUpVector() * halfVSide, cam.GetRightVector()) };
}
