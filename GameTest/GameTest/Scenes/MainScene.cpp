#include "MainScene.h"

#include <iostream>
#include <memory>
#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"

void MainScene::Initialize()
{
	std::cout << "MainScene::Initialize()" << std::endl;

	// Add main camera
	const auto mainCamera = std::make_shared<Camera>(
		Camera::PERSECTIVE,
		XMFLOAT3(0.0f, 5.0f, -10.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
	);
	mainCamera->SetFieldOfView(90.0f);
	mainCamera->SetFarPlane(1000.0f);
	SetMainCamera(mainCamera);

	// Add a directional light
	const auto directionalLight = std::make_shared<DirectionalLight>(
		"Main light",
		XMFLOAT3(0.0f, -1.0f, 0.0f)
	);
	directionalLight->Initialize();
	SetDirectionalLight(directionalLight);

	// Add a cube
	const auto cube = std::make_shared<GameObject>(
		"A cube",
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(1.0f, 1.0f, 1.0f)
	);
	const auto meshRenderer = std::make_shared<PM3D_API::MeshRenderer>("cube.obj");
	cube->AddComponent(meshRenderer);
	cube->Initialize();

	AddChild(cube);
}
