#include "Api/Public/GameHost.h"

#include <stdexcept>

PM3D_API::GameHost::~GameHost()
{
	// delete scene; // Exception 0x80000003 "A breakpoint has been reached." - wtf ???
	
}

void PM3D_API::GameHost::Update(const double deltaTime) const
{
	if (scene)
		scene->Update(deltaTime);
}

void PM3D_API::GameHost::FixedUpdate(const double fixedDeltaTime) const
{
	if (scene)
		scene->FixedUpdate(fixedDeltaTime);
}

void PM3D_API::GameHost::InitializeScene() const
{
	if (scene)
		scene->Initialize();
	else
		throw std::runtime_error("Scene is not set!");
}

void PM3D_API::GameHost::SetDispositif(PM3D::CDispositifD3D11* newDispositif)
{
	dispositif = newDispositif;
}

float PM3D_API::GameHost::GetAspectRatio() const
{
	return static_cast<float>(dispositif->GetLargeur()) / static_cast<float>(dispositif->GetHauteur());
}

float PM3D_API::GameHost::GetScreenWidth() const
{
	return static_cast<float>(dispositif->GetLargeur());
}

float PM3D_API::GameHost::GetScreenHeight() const
{
	return static_cast<float>(dispositif->GetHauteur());
}

void PM3D_API::GameHost::SetScene(const std::shared_ptr<Scene>& newScene)
{
	scene = newScene;
}

void PM3D_API::GameHost::Draw() const
{
	std::cout << "GameHost::Draw()" << std::endl;
	if (scene)
		scene->Draw();
}
