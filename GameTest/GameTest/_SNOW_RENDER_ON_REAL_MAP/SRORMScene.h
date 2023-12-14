#pragma once
#include "Api/Public/GameObject/Basic/BasicSphere.h"
#include "Api/Public/Scene/Scene.h"
#include "Core/Public/Util/Singleton.h"

class SRORMScene final : public PM3D_API::Scene, public PM3D::CSingleton<SRORMScene>
{
public:
    SRORMScene() = default;

    void InitializePhysics() override;
    void InitializeCamera() override;
    void InitializeLights() override;
    void InitializeObjects() override;

private:
    std::unique_ptr<PM3D_API::BasicSphere>::pointer spherePtr;
};
