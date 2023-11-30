#include "ShadowtestScene.h"

#include "Api/Private/Light/Shadow/ShadowProcessor.h"
#include "Api/Public/GameObject/Basic/BasicCube.h"
#include "Api/Public/GameObject/Basic/BasicPlane.h"
#include "Api/Public/Input/Input.h"

void ShadowtestScene::InitializeCamera()
{
	auto camera = std::make_unique<PM3D_API::Camera>(
		"Camera",
		PM3D_API::Camera::CameraType::PERSECTIVE,
		DirectX::XMFLOAT3(0.0f, 5.0f, -10.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	camera->SetClearColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	camera->SetFieldOfView(45.f);
	camera->SetNearDist(0.1f);
	camera->SetFarDist(100.f);

	SetMainCamera(std::move(camera));
}

void ShadowtestScene::InitializeLights()
{
	auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
		"Directional light",
		XMFLOAT3(-1.0f, -1.0f, 0.0f)
	);
	const auto directionalLightPtr = directionalLight.get();
	AddLight(std::move(directionalLight));

	directionalLightPtr->SetWorldPosition({10.f, 10.f, 0.f});
	directionalLightPtr->SetIntensity(1.0f);
	directionalLightPtr->Initialize();
}

void ShadowtestScene::InitializeObjects()
{
	auto cube = std::make_unique<PM3D_API::BasicCube>(
		"Cube",
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(10.0f, 0.1f, 10.0f)
	);
	cube->Initialize();
	AddChild(std::move(cube));

	auto tree = std::make_unique<GameObject>("tree");
	tree->SetWorldScale({5.0f, 5.0f, 5.0f});
	tree->Initialize();

	auto shader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
	tree->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), "tree_pineTallA.obj"));

	AddChild(std::move(tree));

	auto shadowProcessor = std::make_unique<ShadowProcessor>();
	shadowProcessor->Initialize();
	shadowProcessor->SetScene(this);
	AddComponent(std::move(shadowProcessor));
}
