#pragma once

#include "../GameObject/GameObject.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"

namespace PM3D_API
{
class Scene : public GameObject
{
public:
	Scene() : GameObject("Scene", {0, 0, 0}, {0, 0, 0}, {1, 1, 1}) {}
	Scene(const std::string& name) : GameObject(name, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}) {}
	~Scene() override;
	void Initialize() override;

	void AddChild(std::unique_ptr<GameObject>&& child) override;

	const Camera* GetMainCamera() const { return mainCamera.get(); }
	const DirectionalLight* GetDirectionalLight() const { return directionalLight.get(); }
	
protected:
	void SetMainCamera(std::unique_ptr<Camera>&& newMainCamera) { mainCamera = std::move(newMainCamera); }
	void SetDirectionalLight(std::unique_ptr<DirectionalLight>&& newDirectionalLight) { directionalLight = std::move(newDirectionalLight); }
	
private:
	std::unique_ptr<Camera> mainCamera;
	std::unique_ptr<DirectionalLight> directionalLight;

};
}
