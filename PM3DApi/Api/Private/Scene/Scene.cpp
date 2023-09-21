#include "Api/Public/Scene/Scene.h"

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

void PM3D_API::Scene::AddChild(GameObject* child)
{
	GameObject::AddChild(child);
	child->SetScene(this);
}

