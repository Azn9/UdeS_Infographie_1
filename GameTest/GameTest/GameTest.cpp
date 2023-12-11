#include "GameTest.h"

#include "Scenes/LoadingScene.h"

PM3D_API::GameHost* PM3D_API::GameHost::GetInstance()
{
	static GameTest instance;
	return &instance;
}

void GameTest::Initialize()
{
	SetScene(LoadingScene::GetInstancePtr());
}

void GameTest::InitializePostProcessParam() const
{
	postEffectPlane->enableAllPostEffects();
	
	postEffectPlane->SetShaderVariableValue("distance", 0.1f);
	postEffectPlane->SetShaderVariableValue("vignettePower", 4.f);
	postEffectPlane->SetShaderVariableValue("vignetteColor", XMFLOAT4{0.0f, 0.0f, 0.f, 0.8f});
}
