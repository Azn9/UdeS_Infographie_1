#include "Api/Public/Scene/Scene.h"

#include <iostream>

PM3D_API::Scene::~Scene()
{
	isDeleted = true;
	
	std::cout << "Scene::~Scene()" << std::endl;
	children.clear();
}

void PM3D_API::Scene::Initialize()
{
	std::cout << "Scene::Initialize()" << std::endl;

	scene = this;
	
	GameObject::Initialize();
	
	InitializePhysics();
	InitializeCamera();
	InitializeLights();
	InitializeObjects();
	InitializeUI();
}

void PM3D_API::Scene::SetMainCamera(std::unique_ptr<Camera>&& newMainCamera)
{
	newMainCamera->SetScene(this);
	if(mainCamera != nullptr)
		mainCamera->SetScene(nullptr);
	mainCamera = newMainCamera.get();
	mainCamera->UpdatePostProcessShaderParam();
	GameObject::AddChild(std::move(newMainCamera));
}

void PM3D_API::Scene::SetMainCamera(Camera* newMainCamera)
{
	// Check if newMainCamera is a child of this scene

	const GameObject* parent = newMainCamera->GetParent();
	while (parent != nullptr)
	{
		if (parent == this)
			break;

		parent = parent->GetParent();
	}

	if (parent == nullptr)
	{
		throw std::runtime_error("newMainCamera is not a child of this scene");
	}

	mainCamera = newMainCamera;
}

void PM3D_API::Scene::SetPhysicsResolver(std::unique_ptr<PhysicsResolver>&& newPhysicsResolver)
{
	physicsEnabled = true;
	physicsResolver = newPhysicsResolver.get();
	
	GameObject::AddComponent(std::move(newPhysicsResolver));
}

void PM3D_API::Scene::Update()
{
	if (isDeleted) return;
	GameObject::Update();
}

void PM3D_API::Scene::PhysicsUpdate()
{
	if (isDeleted) return;
	
	GameObject::PhysicsUpdate();

	if (physicsEnabled && physicsResolver)
		physicsResolver->ResolvePhysics();
}

void PM3D_API::Scene::Draw()
{
	if (isDeleted) return;
	GameObject::Draw();

	if (lightsNeedUpdate)
		lightsNeedUpdate = false;
}

void PM3D_API::Scene::DrawSelf() const
{
	LogBeginDrawSelf();
	
	if (isDeleted) return;
	GameObject::DrawSelf();

	LogEndDrawSelf();
}

void PM3D_API::Scene::InitializeUI()
{
	uiCanvas = std::make_unique<UICanvas>();
	uiCanvas->Initialize();
}

void PM3D_API::Scene::AddUiChild(std::unique_ptr<UIObject>&& child) const
{
	if (!uiCanvas)
		throw std::runtime_error("Scene::AddUiChild: uiCanvas is not initialized");

	uiCanvas->AddChild(std::move(child));
}
