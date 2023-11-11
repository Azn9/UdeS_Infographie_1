#pragma once
#include <thread>

#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/UIObject.h"
#include "Api/Public/Input/KeyCode.h"

class LoadingSceneComponent final : public PM3D_API::Component
{
public:
	void Initialize() override;
	void Update() override;
	
private:
	std::thread loadingThread;
	std::atomic_bool isLoaded = false;
	std::atomic_bool wantToChangeScene = false;
	PM3D_API::UIObject* loadingTextPtr = nullptr;
	PM3D_API::UIObject* playButtonPtr = nullptr;
};
