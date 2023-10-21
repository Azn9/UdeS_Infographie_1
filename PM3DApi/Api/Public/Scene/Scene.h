#pragma once

#include <iostream>

#include "../GameObject/GameObject.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"

namespace PM3D_API
{
class Scene : public GameObject
{
public:
	Scene() : GameObject("Scene", {0, 0, 0}, {0, 0, 0}, {1, 1, 1})
	{
	}

	Scene(const std::string& name) : GameObject(name, {0, 0, 0}, {0, 0, 0}, {1, 1, 1})
	{
	}

	~Scene() override;
	
	void Initialize() override;
	void Update(double elapsed) override;
	void FixedUpdate(double elapsed) override;
	void Draw() override;
	void DrawSelf() const override;

	template <typename L, template_extends<Light, L>  = 0>
	void AddLight(std::unique_ptr<L>&& child);

	template <typename T, template_extends<GameObject, T> = 0, template_not_extends<Light, T> = 0, template_not_extends<Camera, T> = 0>
	void AddChild(std::unique_ptr<T>&& child);

	const Camera* GetMainCamera() const { return mainCamera; }
	const std::vector<Light*>& GetLights() const { return lights; }

	void SetLightsNeedUpdate(const bool needUpdate) { lightsNeedUpdate = needUpdate; }
	bool GetLightsNeedUpdate() const { return lightsNeedUpdate; }

protected:
	bool isDeleted = false;
	
	void SetMainCamera(std::unique_ptr<Camera>&& newMainCamera);

private:
	Camera* mainCamera;
	std::vector<Light*> lights;
	bool lightsNeedUpdate = false;

};

template <typename L, template_extends<Light, L>>
void Scene::AddLight(std::unique_ptr<L>&& child)
{
	child->SetScene(this);
	lights.push_back(child.get());
	AddChild(std::move(child));

	lightsNeedUpdate = true;
}

template <typename T, template_extends<GameObject, T>, template_not_extends<Light, T>, template_not_extends<Camera, T>>
void Scene::AddChild(std::unique_ptr<T>&& child)
{ 
	std::cout << "Scene::AddChild(GameObject*) added " << child->GetName() << std::endl;

	child->SetScene(this);
	GameObject::AddChild(std::move(child));
}
}
