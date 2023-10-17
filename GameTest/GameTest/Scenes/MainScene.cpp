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
	auto mainCamera = std::make_unique<Camera>(
		"Main camera",
		Camera::PERSECTIVE,
		XMFLOAT3(0.0f, 5.0f, -10.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	mainCamera->SetFieldOfView(90.0f);
	mainCamera->SetFarPlane(1000.0f);
	SetMainCamera(std::move(mainCamera));

	// Add a directional light
	auto directionalLight = std::make_unique<DirectionalLight>(
		"Main light",
		XMFLOAT3(0.0f, -1.0f, 0.0f)
	);
	directionalLight->SetWorldPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	directionalLight->SetIntensity(100.0f);
	directionalLight->SetColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	directionalLight->Initialize();
	SetDirectionalLight(std::move(directionalLight));

	// Add a cube
	auto cube = std::make_unique<GameObject>(
		"A cube",
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(1.0f, 1.0f, 1.0f)
	);
	auto meshRenderer = std::make_unique<PM3D_API::MeshRenderer>("cube.obj");
	cube->AddComponent(std::move(meshRenderer));
	cube->Initialize();

	AddChild(std::move(cube));
}
