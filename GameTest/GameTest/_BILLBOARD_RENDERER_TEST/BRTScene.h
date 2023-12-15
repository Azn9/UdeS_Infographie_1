#pragma once
#include "Api/Public/Scene/Scene.h"
#include "Core/Public/Util/Singleton.h"

class BRTScene : public PM3D_API::Scene, public PM3D::CSingleton<BRTScene>
{
public:
    BRTScene() = default;

    void InitializeCamera() override;
    void InitializeObjects() override;
    void InitializeLights() override;
};
