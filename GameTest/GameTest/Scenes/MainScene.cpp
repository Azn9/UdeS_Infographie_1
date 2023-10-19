#include "MainScene.h"

#include <iostream>
#include <memory>
#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "../../../PM3DApi/Api/Public/Light/PointLight.h"
#include "../../../PM3DApi/Api/Public/Shader/Shader.h"
#include "../../../PM3DApi/Api/Public/Shader/Basic/DefaultShader.h"

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

	/*
	// Add a directional light
	auto directionalLight = std::make_unique<DirectionalLight>(
		"Main light",
		XMFLOAT3(0.0f, -1.0f, 0.0f)
	);
	directionalLight->SetIntensity(0.0f);
	directionalLight->SetColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
	directionalLight->Initialize();
	AddLight(std::move(directionalLight)); 
	
	// Adds an ambient light
	auto ambientLight = std::make_unique<AmbiantLight>(
		0.2f,
		XMFLOAT3(0.2f, 0.2f, 0.2f)
	);
	ambientLight->Initialize();
	AddLight(std::move(ambientLight));
	*/

	auto pointLight = std::make_unique<PointLight>(
		XMFLOAT3(0.0f, 2.0f, 0.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f)
	);
	pointLight->SetIntensity(1.0f);
	pointLight->Initialize();
	AddLight(std::move(pointLight));

	auto shader = std::make_unique<PM3D_API::DefaultShader>(L"NewShader.fx");

	// Add a cube
	auto cube = std::make_unique<GameObject>(
		"A cube",
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(10.0f, 0.1f, 10.0f)
	);
	auto meshRenderer = std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), "cube.obj");
	cube->AddComponent(std::move(meshRenderer));
	cube->Initialize();

	AddChild(std::move(cube));
}
