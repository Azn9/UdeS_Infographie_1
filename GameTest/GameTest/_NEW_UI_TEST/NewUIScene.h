#pragma once
#include "Api/Public/Scene/Scene.h"
#include "Core/Public/Util/Singleton.h"

class NewUIScene final : public PM3D_API::Scene, public PM3D::CSingleton<NewUIScene>
{
public:
    NewUIScene() = default;

    void InitializeCamera() override;
    void InitializeLights() override;
    void InitializeObjects() override;
    void InitializeUI() override;
};
