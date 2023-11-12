#pragma once
#include "Core/Public/Util/Singleton.h"
#include "Api/Public/Scene/Scene.h"

class MainScene final : public PM3D_API::Scene, public PM3D::CSingleton<MainScene>
{
public:
	MainScene() : Scene("MainScene") {}

	void InitializePhysics() override;
	void InitializeCamera() override;
	void InitializeLights() override;
	void InitializeObjects() override;
	void InitializeUI() override;
	void AddPine(const DirectX::XMFLOAT3& pos);
};
