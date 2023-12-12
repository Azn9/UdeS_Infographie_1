#pragma once
#include "Api/Public/Scene/Scene.h"
#include "Core/Public/Util/Singleton.h"

class DITScene final : public PM3D_API::Scene, public PM3D::CSingleton<DITScene>
{
public:
    DITScene() = default;

    void InitializeCamera() override;
    void InitializeLights() override;
    void InitializeObjects() override;
};
