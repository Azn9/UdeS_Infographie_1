#include "GameTest.h"

#include "Scenes/LoadingScene.h"
#include "_NEW_UI_TEST/NewUIScene.h"

PM3D_API::GameHost* PM3D_API::GameHost::GetInstance()
{
    static GameTest instance;
    return &instance;
}

void GameTest::Initialize()
{
    //SetScene(LoadingScene::GetInstancePtr());
    SetScene(NewUIScene::GetInstancePtr());
}
