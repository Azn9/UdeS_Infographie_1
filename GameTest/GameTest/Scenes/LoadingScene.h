#pragma once
#include "Core/Public/Util/Singleton.h"
#include "Api/Public/Scene/Scene.h"

class LoadingScene final : public PM3D_API::Scene, public PM3D::CSingleton<LoadingScene>
{
public:
    LoadingScene() : Scene("Loading scene...")
    {
    }

    void InitializeCamera() override;
    void InitializeLights() override;
    void InitializeObjects() override;
    void InitializeUI() override;
};
