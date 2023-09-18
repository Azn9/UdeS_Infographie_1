#include "MainScene.h"

#include <iostream>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Component.h"

void MainScene::Initialize()
{
	std::cout << "MainScene::Initialize()" << std::endl;
	
	// Add main camera
	const auto mainCamera = new Camera();
	children.emplace_back(mainCamera);

	// Add a directional light
	
}
