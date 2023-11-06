#include "GameTest.h"

#include "Scenes/LoadingScene.h"

PM3D_API::GameHost* PM3D_API::GameHost::GetInstance()
{
	static GameTest instance;
	return &instance;
}

void GameTest::Initialize()
{
	SetScene(LoadingScene::GetInstancePtr());
}
