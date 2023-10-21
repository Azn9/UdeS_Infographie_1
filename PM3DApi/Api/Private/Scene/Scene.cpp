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
	// Do nothing by default
}

void PM3D_API::Scene::SetMainCamera(std::unique_ptr<Camera>&& newMainCamera)
{
	newMainCamera->SetScene(this);
	mainCamera = newMainCamera.get();
	GameObject::AddChild(std::move(newMainCamera));
}

void PM3D_API::Scene::Update(double elapsed)
{
	if (isDeleted) return;
	GameObject::Update(elapsed);
}

void PM3D_API::Scene::FixedUpdate(double elapsed)
{
	if (isDeleted) return;
	GameObject::FixedUpdate(elapsed);
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
