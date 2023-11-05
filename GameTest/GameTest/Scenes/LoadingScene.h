#pragma once
#include "Scene/Scene.h"
#include "Sprite/AfficheurSprite.h"
#include "Util/Singleton.h"

class LoadingScene final : public PM3D_API::Scene, public PM3D::CSingleton<LoadingScene>
{
public:
    LoadingScene() : Scene("Loading scene...")
    {
    }

    void InitializeCamera() override;
    void InitializeLights() override;
    void InitializeObjects() override;

    void Draw() override;
    void DrawSelf() const override;
private:
    std::atomic_bool isLoaded{false};
    PM3D::CAfficheurSprite* afficheurSprite;
};
