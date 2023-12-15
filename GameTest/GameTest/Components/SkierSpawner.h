#pragma once
#include <DirectXMath.h>
#include <memory>


#include "Api/Public/Component/Component.h"
#include "Api/Public/GameObject/GameObject.h"
#include <Api/Public/Component/Basic/Physics/PhysicsResolver.h>
#include <Core/Public/Mesh/FastobjChargeur.h>
#include <API/Public/Component/Basic/Render/3D/MeshRenderer.h>

#include "Skiers/SkiersMultiRenderer.h"


class SkierSpawner final : public PM3D_API::Component
{
public:
    void Initialize() override;
    void Update() override;
    void DrawSelf() const override;

private:
    void AddSkier(const DirectX::XMFLOAT3& pos, physx::PxU32 id, int checkpoint);
    void SkierWave1();
    void SkierWave2();
    void SkierWave3();
    void SkierWave4();
    void SkierWave5();

    physx::PxU32 _current_id{};
    physx::PxU32 _deletedSkierCount = 0;
    bool _reSpawn = true;

    PM3D_API::Scene* _scene;

    std::unique_ptr<PM3D_API::MeshRenderer> _meshrendererSkier;
    std::unique_ptr<PM3D_API::MeshRenderer> _meshrendererlSki;
    std::unique_ptr<PM3D_API::MeshRenderer> _meshrendererrSki;

    PM3D::FastobjChargeur* _chargeurSkier;
    PM3D::FastobjChargeur* _chargeurlSki;
    PM3D::FastobjChargeur* _chargeurrSki;

    std::unique_ptr<SkiersMultiRenderer>::pointer leftSkiMultiRenderer;
    std::unique_ptr<SkiersMultiRenderer>::pointer rightSkiMultiRenderer;
    std::unique_ptr<SkiersMultiRenderer>::pointer skierMultiRenderer;
};
