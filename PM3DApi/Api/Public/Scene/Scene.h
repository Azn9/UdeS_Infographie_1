#pragma once

#include "../GameObject/GameObject.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"

namespace PM3D_API
{
class Scene : public GameObject, public std::enable_shared_from_this<Scene>
{
public:
	Scene() : GameObject("Scene", {0, 0, 0}, {0, 0, 0}, {1, 1, 1}) {}
	Scene(const std::string& name) : GameObject(name, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}) {}
	~Scene() override;
	void Initialize() override;

	void AddChild(const std::shared_ptr<GameObject>& child) override;

	std::shared_ptr<Camera> GetMainCamera() const { return mainCamera; }
	std::shared_ptr<DirectionalLight> GetDirectionalLight() const { return directionalLight; }
	
protected:
	void SetMainCamera(const std::shared_ptr<Camera>& newMainCamera) { mainCamera = newMainCamera; }
	void SetDirectionalLight(const std::shared_ptr<DirectionalLight>& newDirectionalLight) { directionalLight = newDirectionalLight; }
	
private:
	std::shared_ptr<Camera> mainCamera;
	std::shared_ptr<DirectionalLight> directionalLight;

};
}
