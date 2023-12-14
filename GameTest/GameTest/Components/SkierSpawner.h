#pragma once
#include <DirectXMath.h>
#include <iostream>
#include <memory>
#include <vector>


#include "Api/Public/EventSystem/EventSystem.h"
#include "GameTest/RestartEvent.h"
#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include <Api/Public/Component/Basic/Physics/PhysicsResolver.h>


class SkierSpawner final : public PM3D_API::Component
{
public:
	SkierSpawner()
	{
		PM3D_API::EventSystem::Subscribe([this](const RestartEvent&)
			{
				_reSpawn = true;
				Update();
			});
	}

	void Update() override
	{
		if (_reSpawn) {
			_current_id = 0;
			SkierWave1();
			SkierWave2();
			SkierWave3();
			SkierWave4();
			SkierWave5();
			_reSpawn = false;
		}
	}

private:
	void AddSkier(const DirectX::XMFLOAT3& pos, physx::PxU32 id , int checkpoint);
	void SkierWave1();
	void SkierWave2();
	void SkierWave3();
	void SkierWave4();
	void SkierWave5();

	physx::PxU32 _current_id{};
	bool _reSpawn = true;

};
