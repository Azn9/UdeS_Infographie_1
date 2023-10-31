﻿#include "../Public/GameHost.h"

#include <stdexcept>

#include "Api/Public/Debug/FPSDebugRenderer.h"
#include "Api/Public/Debug/SceneHierarchyDebugRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"

PM3D_API::GameHost::~GameHost()
{
	// delete scene; // Exception 0x80000003 "A breakpoint has been reached." - wtf ???
	debugRenderers.clear();
}

void PM3D_API::GameHost::Update() const
{
	EventSystem::GetInstance().processEvents();
	
	if (scene)
		scene->Update();
}

void PM3D_API::GameHost::PhysicsUpdate() const
{
	if (scene)
		scene->PhysicsUpdate();
}

void PM3D_API::GameHost::InitializeScene()
{
	if (scene)
		scene->Initialize();
	else
		throw std::runtime_error("Scene is not set!");

	// Initialize default debugRenderers
	auto fpsDebugRenderer = std::make_unique<FPSDebugRenderer>();
	AddDebugRenderer(std::move(fpsDebugRenderer));

	auto sceneHierarchyRenderer = std::make_unique<SceneHierarchyDebugRenderer>();
	AddDebugRenderer(std::move(sceneHierarchyRenderer));
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

void PM3D_API::GameHost::SetScene(Scene* newScene)
{
	scene = newScene;
}

void PM3D_API::GameHost::Draw()
{
	if (scene)
		scene->Draw();

	// TODO : add debug switch
	for (const auto& renderer : debugRenderers)
	{
		if (renderer)
			renderer->Draw();
	}
}
