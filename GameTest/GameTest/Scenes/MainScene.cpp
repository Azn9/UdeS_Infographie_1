#include "MainScene.h"

#include <iostream>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"

void MainScene::Initialize()
{
	std::cout << "MainScene::Initialize()" << std::endl;

	// Add main camera
	const auto mainCamera = new Camera(
		Camera::PERSECTIVE,
		{0.0f, 5.0f, -10.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f}
	);
	mainCamera->SetFieldOfView(90.0f);
	mainCamera->SetFarPlane(1000.0f);
	SetMainCamera(mainCamera);

	// Add a directional light
	const auto directionalLight = new DirectionalLight(
		"Main light",
		{0.0f, -1.0f, 0.0f}
		);
	directionalLight->Initialize();
	SetDirectionalLight(directionalLight);

	// Add a cube
	const auto cube = new GameObject(
		"A cube",
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	);
	const auto meshRenderer = new PM3D_API::MeshRenderer("cube.obj");
	cube->AddComponent(meshRenderer);
	cube->Initialize();

	AddChild(cube);
}
