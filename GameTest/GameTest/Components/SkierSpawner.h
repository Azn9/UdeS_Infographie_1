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
#include "Core/Public/Util/Time.h"
#include <Api/Public/EventSystem/RespawnSkierEvent.h>
#include <Api/Public/EventSystem/DeletedSkierEvent.h>

class SkierSpawner final : public PM3D_API::Component
{
public:
	SkierSpawner()
	{
		PM3D_API::EventSystem::Subscribe([this](const RespawnSkierEvent&)
			{
				_reSpawn = true;
				Update();
			});
		PM3D_API::EventSystem::Subscribe([this](const DeletedSkierEvent&)
			{
				_deletedSkierCount++;
				if(_deletedSkierCount == _current_id*3)
					PM3D_API::EventSystem::Publish(RespawnSkierEvent());
			});
	}

	void Update() override
	{
		if (_reSpawn) {
			_current_id = 0;
			_deletedSkierCount = 0;
			SkierWave1();
			SkierWave2();
			SkierWave3();
			SkierWave4();
			SkierWave5();
			_reSpawn = false;
			PM3D::Time::GetInstance().SetTimeScale(1.f);
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
	physx::PxU32 _deletedSkierCount = 0;
	bool _reSpawn = true;

};
