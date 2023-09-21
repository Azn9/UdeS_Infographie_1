#pragma once

#include "../GameObject/GameObject.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"

namespace PM3D_API
{
class Scene : public GameObject
{
public:
	Scene() : GameObject({0, 0, 0}, {0, 0, 0}, {0, 0, 0}) {}
	~Scene() override;
	virtual void Initialize();

	void AddChild(GameObject* child) override;

	Camera* GetMainCamera() const { return mainCamera; }
	DirectionalLight* GetDirectionalLight() const { return directionalLight; }
	
protected:
	void SetMainCamera(Camera* newMainCamera) { mainCamera = newMainCamera; }
	void SetDirectionalLight(DirectionalLight* newDirectionalLight) { directionalLight = newDirectionalLight; }
	
private:
	Camera* mainCamera;
	DirectionalLight* directionalLight;

};
}
