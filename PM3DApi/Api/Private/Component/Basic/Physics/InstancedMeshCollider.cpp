#include "Api/Public/Component/Basic/Physics/InstancedMeshCollider.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/GameHost.h"
#include <PxPhysicsAPI.h>
#include "Core/Public/Mesh/FastobjChargeur.h"
#include <algorithm>
#include <vector>

#include "Api/Public/Component/Basic/Render/3D/InstancedMeshRenderer.h"
#include "Api/Public/Util/FilterGroup.h"

using namespace physx;

void PM3D_API::InstancedMeshCollider::Initialize()
{
    const auto pxPhysics = parentObject->GetScene()->GetPhysicsResolver()->GetPhysics();

    const auto meshRenderer = parentObject->GetComponent<InstancedMeshRenderer>();

    if (!meshRenderer)
    {
        throw std::runtime_error("MeshCollider::Initialize - No mesh renderer on the parent object");
    }

    const auto chargeur = static_cast<PM3D::FastobjChargeur*>(meshRenderer->getChargeur());

    std::vector<XMFLOAT3> verts = chargeur->getPositionArray();

    if (!verts.empty())
    {
        verts.erase(verts.begin());
    }

    const auto nbVerts = static_cast<PxU32>(verts.size());

    std::vector<PxU32> indices32;

    std::vector<int> indices = chargeur->getIndexFaces();

    std::ranges::transform(indices, std::back_inserter(indices32), [](const int intValue)
    {
        return static_cast<PxU32>(intValue);
    });

    for (int i = 0; i < indices32.size() / 3; ++i)
    {
        const auto temp = indices32[i * 3];
        indices32[i * 3] = indices32[i * 3 + 1];
        indices32[i * 3 + 1] = temp;
    }

    const auto triCount = static_cast<PxU32>(indices32.size() / 3);

    const PxTolerancesScale toleranceScale;
    PxCookingParams params(toleranceScale);
    // disable mesh cleaning - perform mesh validation on development configurations
    params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
    // disable edge precompute, edges are set for each triangle, slows contact generation
    params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

    const auto physicsResolver = parentObject->GetScene()->GetPhysicsResolver();

    for (const auto& instance : meshRenderer->getInstances())
    {
        const auto position = instance.position;
        const auto rotation = instance.rotation;
        const auto scale = instance.scale;

        auto newVerts = std::vector<XMFLOAT3>(nbVerts);
        std::ranges::transform(verts, std::begin(newVerts), [scale](const XMFLOAT3& vect)
        {
            return XMFLOAT3(vect.x * scale.x, vect.y * scale.y, vect.z * scale.z);
        });

        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = nbVerts;
        meshDesc.points.stride = sizeof(PxVec3);
        meshDesc.points.data = newVerts.data();

        meshDesc.triangles.count = triCount; // nb triangles
        meshDesc.triangles.stride = 3 * sizeof(PxU32);
        meshDesc.triangles.data = indices32.data();

        PxTriangleMesh* triangleMesh = PxCreateTriangleMesh(params, meshDesc, pxPhysics->getPhysicsInsertionCallback());

        const auto shape = pxPhysics->createShape(PxTriangleMeshGeometry(triangleMesh), *material, true);

        PxFilterData filterData;
        filterData.word0 = FilterGroup::eOBSTACLE;
        shape->setSimulationFilterData(filterData);

        const auto quaternionRotation = Quaternion(rotation);

        const auto globalPos = physx::PxTransform(
            physx::PxVec3(
                position.x,
                position.y,
                position.z
            ),
            physx::PxQuat(
                quaternionRotation.x,
                quaternionRotation.y,
                quaternionRotation.z,
                quaternionRotation.w
            )
        );

        const auto actor = physx::PxCreateStatic(
            *physicsResolver->GetPhysics(),
            globalPos,
            *shape
        );

        if (!actor)
        {
            throw std::runtime_error("InstancedMeshCollider::Initialize() - Failed to create actor");
        }

        physicsResolver->GetScene()->addActor(*actor);
        physicsResolver->AddActor(actor);
    }
}
