#include "../../Public/Scene/Scene.h"

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
	
	GameObject::Initialize();
	
	InitializePhysics();
	InitializeCamera();
	InitializeLights();
	InitializeObjects();
}

void PM3D_API::Scene::SetMainCamera(std::unique_ptr<Camera>&& newMainCamera)
{
	newMainCamera->SetScene(this);
	mainCamera = newMainCamera.get();
	GameObject::AddChild(std::move(newMainCamera));
}

void PM3D_API::Scene::SetMainCamera(Camera* newMainCamera)
{
	// Check if newMainCamera is a child of this scene

	const auto it = std::find_if(
		children.begin(),
		children.end(),
		[newMainCamera](const std::unique_ptr<GameObject>& child) {
			return child.get() == newMainCamera;
		}
	);
	
	if (it == children.end())
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
