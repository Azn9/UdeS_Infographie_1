#include "../../../../Public/Component/Basic/Physics/MeshCollider.h"
#include "../../../../Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../../Public/GameHost.h"
#include <PxPhysicsAPI.h>
#include "../PM3DApi/Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "../PetitMoteur3D/Core/Public/Mesh/FastobjChargeur.h"
#include <algorithm>
#include <vector>

using namespace physx;

void PM3D_API::MeshCollider::Initialize()
{
	Collider::Initialize();

	const auto pxPhysics = GameHost::GetInstance()->GetScene()->GetPhysicsResolver()->GetPhysics();

	const auto worldScale = parentObject->GetWorldScale();

	const auto meshRenderer = parentObject->GetComponent<MeshRenderer>();

	if (!meshRenderer) 
	{
		throw std::runtime_error("MeshCollider::Initialize - No mesh renderer on the parent object");
	}

	PM3D::FastobjChargeur* chargeur = static_cast<PM3D::FastobjChargeur*>(meshRenderer->getChargeur());

	std::vector<XMFLOAT3> verts = chargeur->getPositionArray();

	if (!verts.empty()) {
		verts.erase(verts.begin());
	}

	PxU32 nbVerts = verts.size();

	std::vector<PxU32> indices32;
	
	std::vector<int> indices = chargeur->getIndexFaces();

	std::transform(indices.begin(), indices.end(), std::back_inserter(indices32), [](int intValue) {
		return static_cast<PxU32>(intValue);
		});

	for (int i = 0; i < indices32.size() /3; ++i) {
		auto temp = indices32[i * 3];
		indices32[i * 3] = indices32[i * 3 + 1];
		indices32[i * 3 + 1] = temp;
	}
	
	PxU32 triCount = indices32.size() / 3;

	for_each(begin(verts), end(verts), [worldScale](XMFLOAT3& vect) {
		vect.x *= worldScale.x;
		vect.y *= worldScale.y;
		vect.z *= worldScale.z;
		});

	PxTolerancesScale scale;
	PxCookingParams params(scale);
	// disable mesh cleaning - perform mesh validation on development configurations
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	// disable edge precompute, edges are set for each triangle, slows contact generation
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;


	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = nbVerts;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = verts.data();

	meshDesc.triangles.count = triCount; // nb triangles
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = indices32.data(); 

	PxTriangleMesh* triangleMesh = PxCreateTriangleMesh(params, meshDesc, pxPhysics->getPhysicsInsertionCallback());

	shape = pxPhysics->createShape(PxTriangleMeshGeometry(triangleMesh), *material, true);

	const auto rigidbody = parentObject->GetComponent<Rigidbody>(); // Ne peut pas être null, vérifié dans Collider::Initialize()

	const auto actor = rigidbody->GetActor();

	PxShape* baseShape;
	actor->getShapes(&baseShape, 1, 0);

	actor->detachShape(*baseShape);
	actor->attachShape(*shape);

	// Set position and rotation
	const auto worldPosition = parentObject->GetWorldPosition();
	const auto worldRotation = parentObject->GetWorldRotationQuaternion();

	const auto globalPos = physx::PxTransform(
		physx::PxVec3(worldPosition.x, worldPosition.y, worldPosition.z),
		physx::PxQuat(worldRotation.x, worldRotation.y, worldRotation.z, worldRotation.w)
	);

	actor->setGlobalPose(globalPos);
}