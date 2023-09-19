#pragma once
#include "../Component/Component.h"
#include "Api/Public/Camera/Camera.h"

namespace PM3D_API
{
class Scene : public Component
{
public:
	Scene() : Component({0, 0, 0}, {0, 0, 0}, {0, 0, 0}) {}
	~Scene() override;
	virtual void Initialize();

	Camera* GetMainCamera() const { return mainCamera; }
	
protected:
	void SetMainCamera(Camera* newMainCamera) { mainCamera = newMainCamera; }
	void SetDirectionalLight(DirectionalLight* newDirectionalLight) { directionalLight = newDirectionalLight; }
	
private:
	Camera* mainCamera;

};
}
