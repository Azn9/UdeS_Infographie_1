#pragma once
#include "Api/Public/Scene/Scene.h"
#include "Core/Public/Util/Singleton.h"

class SRScene final : public PM3D_API::Scene, public PM3D::CSingleton<SRScene>
{
public:
    SRScene() = default;

    void InitializeCamera() override;
    void InitializeLights() override;
    void InitializeObjects() override;
};
