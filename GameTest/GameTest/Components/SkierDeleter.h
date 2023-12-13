#pragma once
#include <DirectXMath.h>
#include <iostream>

#include "Api/Public/EventSystem/EventSystem.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Objects/LoadingScene/CustomCube.h"
#include "Core/Imgui/imgui.h"
#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"

class SkierDeleter final : public PM3D_API::Component
{
public:
	SkierDeleter()
	{
		PM3D_API::EventSystem::Subscribe([this](const RestartEvent&)
			{
				_resetRequested = true;
			});
	}

	void Update() override
	{
		if (_resetRequested) {
			auto pt = parentObject->DetachFromParent();
			physx::PxRigidDynamic* rigidDyn = pt->GetComponent<PM3D_API::Rigidbody>()->getRigidDynamic();
			rigidDyn->release();
			_resetRequested = false;
		}
	}

private:
	bool _resetRequested = false;
};
