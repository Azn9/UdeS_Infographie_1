#include "Api//Public/Camera/Frustrum.h"
#include "Api/Public/GameHost.h"
#include "Api/Public/Util/Util.h"

using namespace Util;

void PM3D_API::Frustrum::SetPlanes(const Camera& cam)
{
    const float halfVSide = cam.getFarDist() * tanf(cam.getFieldOfView() * .5f);
    const float halfHSide = halfVSide * GameHost::GetInstance()->GetAspectRatio();
    const DirectX::XMVECTOR frontMultFar = DirectX::XMVectorScale(cam.GetForwardVector(), cam.getFarDist());

    const DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&cam.GetWorldPosition());
    
    nearPlane = { -cam.GetForwardVector(), camPos + cam.GetForwardVector() * cam.getNearDist()};
    
    farPlane = { cam.GetForwardVector(), camPos + frontMultFar };
    
    rightPlane = { DirectX::XMVector3Cross(frontMultFar - cam.GetRightVector() * halfHSide, cam.GetUpVector()), camPos };
    
    leftPlane = {DirectX::XMVector3Cross(cam.GetUpVector(),frontMultFar + cam.GetRightVector() * halfHSide), camPos };
    
    topPlane = { DirectX::XMVector3Cross(cam.GetRightVector(), frontMultFar - cam.GetUpVector() * halfVSide), camPos };
    
    bottomPlane = {DirectX::XMVector3Cross(frontMultFar + cam.GetUpVector() * halfVSide, cam.GetRightVector()), camPos };
}

void PM3D_API::Frustrum::DrawDebugInfo() const
{
    DrawDebugVector3("nearNormal", nearPlane.normal);
    DrawDebugVector3("farNormal", farPlane.normal);
    DrawDebugVector3("rightNormal", rightPlane.normal);
    DrawDebugVector3("leftNormal", leftPlane.normal);
    DrawDebugVector3("topNormal", topPlane.normal);
    DrawDebugVector3("bottomNormal", bottomPlane.normal);
}
