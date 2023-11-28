#pragma once

#include <iostream>

#include "../GameObject/GameObject.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../Component/Basic/Physics/PhysicsResolver.h"
#include "../GameObject/UICanvas.h"

namespace PM3D_API
{
class Scene : public GameObject
{
public:
	Scene() : GameObject("Scene", {0, 0, 0}, {0, 0, 0}, {1, 1, 1})
	{
		std::cout << "Scene::Scene()" << std::endl;
	}

	Scene(const std::string& name) : GameObject(name, {0, 0, 0}, {0, 0, 0}, {1, 1, 1})
	{
		std::cout << "Scene::Scene(const std::string&)" << std::endl;
	}

	~Scene() override;
	
	void Initialize() override;
	void Update() override;
	void PhysicsUpdate() override;
	void Draw() override;
	void DrawShadow(const Camera& camera) override;

	virtual void InitializePhysics() {}
	virtual void InitializeCamera() {}
	virtual void InitializeLights() {}
	virtual void InitializeObjects() {}
	virtual void InitializeUI();

	template <typename L, template_extends<Light, L>  = 0>
	void AddLight(std::unique_ptr<L>&& child);

	template <typename T,
		template_extends<GameObject, T> = 0,
		template_not_extends<Light, T> = 0,
		template_not_extends<Camera, T> = 0
	>
	void AddChild(std::unique_ptr<T>&& child);

	void AddUiChild(std::unique_ptr<UIObject>&& child) const;

	Camera* GetMainCamera() const { return mainCamera; }
	PhysicsResolver* GetPhysicsResolver() const { return physicsResolver; }
	UICanvas* GetUICanvas() const { return uiCanvas.get(); }
	
	const std::vector<Light*>& GetLights() const { return lights; }

	void SetLightsNeedUpdate(const bool needUpdate) { lightsNeedUpdate = needUpdate; }
	bool GetLightsNeedUpdate() const { return lightsNeedUpdate; }

	void SetMainCamera(Camera* newMainCamera);
protected:
	bool isDeleted = false;

	PhysicsResolver* physicsResolver;
	std::unique_ptr<UICanvas> uiCanvas;
	
	void SetMainCamera(std::unique_ptr<Camera>&& newMainCamera);
	void SetPhysicsResolver(std::unique_ptr<PhysicsResolver>&& newPhysicsResolver);

	void DrawSelf() const override;

private:
	Camera* mainCamera;
	
	std::vector<Light*> lights;
	
	bool lightsNeedUpdate = false;
	bool physicsEnabled = false;

};

template <typename L, template_extends<Light, L>>
void Scene::AddLight(std::unique_ptr<L>&& child)
{
	child->SetScene(this);
	lights.push_back(child.get());
	GameObject::AddChild(std::move(child));

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
