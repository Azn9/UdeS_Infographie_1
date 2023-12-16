#include "Api//Public/Camera/Frustrum.h"
#include "Api/Public/GameHost.h"

using namespace DirectX;
using namespace Util;

void PM3D_API::Frustrum::SetPlanes(const Camera& cam)
{
    const float halfVSide = cam.getFarDist() * tanf(cam.getFieldOfView() * .5f);
    const float halfHSide = halfVSide * GameHost::GetInstance()->GetAspectRatio();
    const DirectX::XMVECTOR frontMultFar = DirectX::XMVectorScale(VECTORFORWARD, cam.getFarDist());


    nearPlane = {-VECTORFORWARD, VECTORFORWARD * cam.getNearDist()};

    farPlane = {VECTORFORWARD, frontMultFar};

    rightPlane = {-DirectX::XMVector3Cross(frontMultFar - VECTORRIGHT * halfHSide, VECTORUP), VECTORZERO};

    leftPlane = {-DirectX::XMVector3Cross(VECTORUP, frontMultFar + VECTORRIGHT * halfHSide), VECTORZERO};

    topPlane = {-DirectX::XMVector3Cross(VECTORRIGHT, frontMultFar - VECTORUP * halfVSide), VECTORZERO};

    bottomPlane = {-DirectX::XMVector3Cross(frontMultFar + VECTORUP * halfVSide, VECTORRIGHT), VECTORZERO};
}

bool PM3D_API::Frustrum::ContainsSphere(const DirectX::XMVECTOR& point, const float& radius) const
{
    //std::cout << leftPlane.GetNormal().m128_f32[2] << '\n';
    if (leftPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if (rightPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if (nearPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if (topPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if (bottomPlane.IsSphereInFrontOfPlane(point, radius))
        return false;
    if (farPlane.IsSphereInFrontOfPlane(point, radius))
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
