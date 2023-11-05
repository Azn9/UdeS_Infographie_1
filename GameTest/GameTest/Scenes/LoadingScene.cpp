#include "LoadingScene.h"

#include <thread>

#include "MainScene.h"
#include "GameTest/CustomCube.h"
#include "Sprite/AfficheurSprite.h"

void LoadingScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSECTIVE,
        DirectX::XMFLOAT3(0.0f, 5.0f, -10.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarPlane(1000.0f);
    SetMainCamera(std::move(mainCamera));
}

void LoadingScene::InitializeLights()
{
}

void LoadingScene::InitializeObjects()
{
    afficheurSprite = new PM3D::CAfficheurSprite(PM3D_API::GameHost::GetInstance()->GetDispositif());
    afficheurSprite->AjouterSprite("tree02s.dds", 200,400);


    // Load scene in the background
    std::thread loadingThread([this]()
    {
        const auto mainScenePtr = MainScene::GetInstancePtr();
        mainScenePtr->Initialize();
        
        isLoaded = true;
    });
    loadingThread.detach();
}

void LoadingScene::Draw()
{
    if (!isLoaded)
    {
        Scene::Draw();
        return;
    }

    PM3D_API::GameHost::GetInstance()->SetScene(MainScene::GetInstancePtr());
    MainScene::GetInstancePtr()->Draw();
}

void LoadingScene::DrawSelf() const
{
    afficheurSprite->Draw();
}
