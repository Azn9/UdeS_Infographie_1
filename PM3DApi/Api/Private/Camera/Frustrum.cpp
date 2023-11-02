#include "Api//Public/Camera/Frustrum.h"
#include "Api/Public/GameHost.h"


using namespace Util;

void PM3D_API::Frustrum::SetPlanes(const Camera& cam)
{
    const float halfVSide = cam.getFarDist() * tanf(cam.getFieldOfView() * .5f);
    const float halfHSide = halfVSide * GameHost::GetInstance()->GetAspectRatio();
    const DirectX::XMVECTOR frontMultFar = DirectX::XMVectorScale(cam.GetForwardVector(), cam.getFarDist());

    const DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&cam.GetWorldPosition());
    
    nearPlane = { -cam.GetForwardVector(), camPos + cam.GetForwardVector() * cam.getNearDist()};
    
    farPlane = { cam.GetForwardVector(), camPos + frontMultFar };
    
    rightPlane = { -DirectX::XMVector3Cross(frontMultFar - cam.GetRightVector() * halfHSide, cam.GetUpVector()), camPos };
    
    leftPlane = {-DirectX::XMVector3Cross(cam.GetUpVector(),frontMultFar + cam.GetRightVector() * halfHSide), camPos };
    
    topPlane = { -DirectX::XMVector3Cross(cam.GetRightVector(), frontMultFar - cam.GetUpVector() * halfVSide), camPos };
    
    bottomPlane = {-DirectX::XMVector3Cross(frontMultFar + cam.GetUpVector() * halfVSide, cam.GetRightVector()), camPos };
}

bool PM3D_API::Frustrum::ContainsSphere(const DirectX::XMVECTOR& point, const float& radius) const
{
    //std::cout << leftPlane.GetNormal().m128_f32[2] << '\n';
    if(leftPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if(rightPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if(nearPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if(topPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if(bottomPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if(farPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    return true;
}


void PM3D_API::Frustrum::DrawDebugInfo() const
{
    DrawDebugVector3("nearNormal", nearPlane.GetNormal());
    DrawDebugVector3("farNormal", farPlane.GetNormal());
    DrawDebugVector3("rightNormal", rightPlane.GetNormal());
    DrawDebugVector3("leftNormal", leftPlane.GetNormal());
    DrawDebugVector3("topNormal", topPlane.GetNormal());
    DrawDebugVector3("botNormal", bottomPlane.GetNormal());
}
