#include "MainScene.h"

#include <iostream>
#include <memory>
#include <memory>

#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/EmptyGameObject.h"
#include "../../../PM3DApi/Api/Public/GameObject/Basic/Plane.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "../../../PM3DApi/Api/Public/Light/PointLight.h"
#include "../../../PM3DApi/Api/Public/Shader/Shader.h"
#include "../../../PM3DApi/Api/Public/Shader/Basic/DefaultShader.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/LightMoverComponent.h"

void MainScene::Initialize()
{
	std::cout << "MainScene::Initialize()" << std::endl;

	// Add main camera
	auto mainCamera = std::make_unique<Camera>(
		"Main camera",
		Camera::PERSECTIVE,
		XMFLOAT3(0.0f, 5.0f, -1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	mainCamera->SetFieldOfView(45.0f);
	mainCamera->SetFarPlane(1000.0f);

	auto cameraMover = std::make_unique<CameraMoverComponent>();
	mainCamera->AddComponent(std::move(cameraMover));
	
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

	auto redLight = std::make_unique<PointLight>(
		"Red light",
		XMFLOAT3(-2.5f, 20.0f, -2.5f),
		XMFLOAT3(1.0f, 0.0f, 0.0f)
	);

	//auto redLightMover = std::make_unique<LightMoverComponent>(0);
	//redLight->AddComponent(std::move(redLightMover));
	
	redLight->SetIntensity(1.0f);
	redLight->Initialize();
	AddLight(std::move(redLight));

	auto blueLight = std::make_unique<PointLight>(
		"Blue light",
		XMFLOAT3(2.5f, 20.0f, -2.5f),
		XMFLOAT3(0.0f, 1.0f, 0.0f)
	);

	//auto blueLightMover = std::make_unique<LightMoverComponent>(120);
	//blueLight->AddComponent(std::move(blueLightMover));
	
	blueLight->SetIntensity(1.0f);
	blueLight->Initialize();
	AddLight(std::move(blueLight));

	auto greenLight = std::make_unique<PointLight>(
		"Green light",
		XMFLOAT3(0.0f, 20.0f, 1.83f),
		XMFLOAT3(0.0f, 0.0f, 1.0f)
	);

	//auto greenLightMover = std::make_unique<LightMoverComponent>(240);
	//greenLight->AddComponent(std::move(greenLightMover));
	
	greenLight->SetIntensity(1.0f);
	greenLight->Initialize();
	AddLight(std::move(greenLight));

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
	

	auto plane = std::make_unique<Plane>(
		"Test plane",
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(10.0f, 0.1f, 10.0f)
	);
	plane->Initialize();

	AddChild(std::move(plane));

	auto testParent = std::make_unique<EmptyGameObject>("Test parent");
	auto testParent2 = std::make_unique<EmptyGameObject>("Test parent 2");
	auto testChild = std::make_unique<EmptyGameObject>("Test child");

	testParent2->AddChild(std::move(testChild));
	testParent->AddChild(std::move(testParent2));
	AddChild(std::move(testParent));
}
