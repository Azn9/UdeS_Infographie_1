#include "Api//Public/Camera/Frustrum.h"
#include "Api/Public/GameHost.h"

void PM3D_API::Frustrum::SetPlanes(const PM3D_API::Camera& cam)
{
    const float halfVSide = cam.getFarDist() * tanf(cam.getFieldOfView() * .5f);
    const float halfHSide = halfVSide * PM3D_API::GameHost::GetInstance()->GetAspectRatio();
    const DirectX::XMVECTOR frontMultFar = DirectX::XMVectorScale(cam.GetForwardVector(), cam.getFarDist());

    frustum.nearFace = { cam.Position + zNear * cam.Front, cam.Front };
    
    near = Plane{ DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&cam.GetWorldPosition()), DirectX::XMVectorScale(cam.GetForwardVector(), cam.getNearDist())), cam.GetForwardVector()};
    far = { cam.Position + frontMultFar, -cam.Front };
    right = { cam.Position,
                            glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up) };
    left = { cam.Position,
                            glm::cross(cam.Up,frontMultFar + cam.Right * halfHSide) };
    top = { cam.Position,
                            glm::cross(cam.Right, frontMultFar - cam.Up * halfVSide) };
    bottom = { cam.Position,
                            glm::cross(frontMultFar + cam.Up * halfVSide, cam.Right) };
}
