#include "../../Public/Scene/Scene.h"

#include <iostream>

PM3D_API::Scene::~Scene()
{
	std::cout << "Scene::~Scene()" << std::endl;
	children.clear();
}

void PM3D_API::Scene::Initialize()
{
	// Do nothing by default
}

void PM3D_API::Scene::AddChild(std::unique_ptr<GameObject>&& child)
{
	std::cout << "Scene::AddChild(GameObject*) added " << child->GetName() << std::endl;

	child->SetScene(this);
	GameObject::AddChild(std::move(child));
}

