#include "../../../../Public/Component/Basic/Physics/MeshCollider.h"
#include "../../../../Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../../Public/GameHost.h"
#include <limits>
#include <PxPhysicsAPI.h>
#include "../PM3DApi/Api/Public/Component/Basic/Render/MeshRenderer.h"
#include "../PetitMoteur3D/Core/Public/Mesh/FastobjChargeur.h"

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
	PxU32 nbVerts = verts.size();

	std::vector<PxU32> indices32;
	
	std::vector<int> indices = chargeur->getTabIndex();

	std::transform(indices.begin(), indices.end(), std::back_inserter(indices32), [](int intValue) {
		return static_cast<PxU32>(intValue);
		});
	
	PxU32 triCount = 0;// indices32.size();


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