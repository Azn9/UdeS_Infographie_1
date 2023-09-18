#include "GameTest.h"

#include <iostream>

#include "Scenes/MainScene.h"

PM3D_API::GameHost* PM3D_API::GameHost::GetInstance()
{
	static GameTest instance;
	return &instance;
}

GameTest::GameTest()
{
	std::cout << "GameTest::GameTest()" << std::endl;

}

void GameTest::Initialize()
{
	std::cout << "GameTest::Initialize()" << std::endl;
	
	SetScene(MainScene::GetInstancePtr());
}
