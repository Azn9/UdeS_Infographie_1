#pragma once
#include "Api/Public/GameObject/Basic/BasicSphere.h"
#include "Core/Public/Util/Singleton.h"
#include "Api/Public/Scene/Scene.h"
#include "GameTest/Objects/Sphere/Sphere.h"

class MainScene final : public PM3D_API::Scene, public PM3D::CSingleton<MainScene>
{
public:
    MainScene() : Scene("MainScene")
    {
    }

    void InitializePhysics() override;
    void InitializeCamera() override;
    void InitializeLights() override;
    void InitializeObjects() override;
    void InitializeUI() override;

    auto GetShpere() const { return spherePtr; }

private:
    void AddPine(const DirectX::XMFLOAT3& pos);
    std::unique_ptr<Sphere>::pointer spherePtr;
};
