#pragma once
#include "Api/Public/Scene/Scene.h"
#include "Core/Public/Util/Singleton.h"

class ShadowtestScene : public PM3D_API::Scene, public PM3D::CSingleton<ShadowtestScene>
{
public:
	void InitializeCamera() override;
	void InitializeLights() override;
	void InitializeObjects() override;

};
