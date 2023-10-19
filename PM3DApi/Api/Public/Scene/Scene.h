#pragma once

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

	template <typename L, template_extends<Light, L>  = 0>
	void AddLight(std::unique_ptr<L>&& child);

	void AddChild(std::unique_ptr<GameObject>&& child) override;

	const Camera* GetMainCamera() const { return mainCamera.get(); }
	const std::vector<std::unique_ptr<Light>>& GetLights() const { return lights; }

protected:
	void SetMainCamera(std::unique_ptr<Camera>&& newMainCamera) { mainCamera = std::move(newMainCamera); }

private:
	std::unique_ptr<Camera> mainCamera;
	std::vector<std::unique_ptr<Light>> lights;

};

template <typename L, template_extends<Light, L>>
void Scene::AddLight(std::unique_ptr<L>&& child)
{
	child->SetScene(this);
	lights.push_back(std::move(child));
}
}
