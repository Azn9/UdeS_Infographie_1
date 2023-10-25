#pragma once
#include "../../../PetitMoteur3D/Core/Public/Util/Singleton.h"
#include "../../../PM3DApi/Api/Public/Scene/Scene.h"

class MainScene final : public PM3D_API::Scene, public PM3D::CSingleton<MainScene>
{
public:
	MainScene() : Scene("MainScene") {}

	void InitializePhysics() override;
	void InitializeCamera() override;
	void InitializeLights() override;
	void InitializeObjects() override;
};
