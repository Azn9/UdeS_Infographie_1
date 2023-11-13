#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/GameHost.h"

void PM3D_API::Rigidbody::Initialize()
{
	const auto physicsResolver = parentObject->GetScene()->GetPhysicsResolver();

	const auto globalPos = physx::PxTransform(
		physx::PxVec3(
			parentObject->GetWorldPosition().x,
			parentObject->GetWorldPosition().y,
			parentObject->GetWorldPosition().z
		),
		physx::PxQuat(
			parentObject->GetWorldRotationQuaternion().x,
			parentObject->GetWorldRotationQuaternion().y,
			parentObject->GetWorldRotationQuaternion().z,
			parentObject->GetWorldRotationQuaternion().w
		)
	);

	if (isStatic)
	{
		actor = physx::PxCreateStatic(
			*physicsResolver->GetPhysics(),
			globalPos,
			physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), // Sera remplacé dans les colliders
			*physicsResolver->GetDefaultMaterial()
		);
	} else
	{
		actor = physx::PxCreateDynamic(
			*physicsResolver->GetPhysics(),
			globalPos,
			physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), // Sera remplacé dans les colliders
			*physicsResolver->GetDefaultMaterial(),
			1.0f
		);
	}

	if (!actor)
	{
		throw std::runtime_error("Rigidbody::Initialize() - Failed to create actor");
	}

	physicsResolver->GetScene()->addActor(*actor);
	physicsResolver->AddActor(actor);
}

void PM3D_API::Rigidbody::UpdateGlobalPose() const
{
	const auto newGlobalPos = physx::PxTransform(
		physx::PxVec3(parentObject->GetWorldPosition().x, parentObject->GetWorldPosition().y,
			parentObject->GetWorldPosition().z),
		physx::PxQuat(parentObject->GetWorldRotationQuaternion().x, parentObject->GetWorldRotationQuaternion().y,
			parentObject->GetWorldRotationQuaternion().z, parentObject->GetWorldRotationQuaternion().w)
	);

	actor->setGlobalPose(newGlobalPos);

	//std::cout << "Rigidbody::UpdateGlobalPos() on " << parentObject->GetName() << ": " << newGlobalPos.p.x << ", " << newGlobalPos.p.y << ", " << newGlobalPos.p.z << std::endl;
}

void PM3D_API::Rigidbody::UpdateRenderPos() const
{
	if (!actor) return;

	const auto globalPos = actor->getGlobalPose();
	const auto pos = globalPos.p;
	const auto rot = globalPos.q;

	parentObject->SetWorldPositionViaPhysic({pos.x, pos.y, pos.z});
	parentObject->SetWorldRotationViaPhysic({rot.x, rot.y, rot.z, rot.w});
}

void PM3D_API::Rigidbody::SetTranslationLock(const Axis axis)
{
	const auto physicsResolver = GameHost::GetInstance()->GetScene()->GetPhysicsResolver();
	const auto pxScene = physicsResolver->GetScene();

	if (fixedJoint)
	{
		fixedJoint->release();
	}

	fixedJoint = physx::PxD6JointCreate(
		*physicsResolver->GetPhysics(),
		actor,
		physx::PxTransform(physx::PxIdentity),
		nullptr,
		physx::PxTransform(physx::PxIdentity)
	);

	fixedJoint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eFREE);
	fixedJoint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eFREE);
	fixedJoint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eFREE);

	switch (axis)
	{
	case Axis::X:
		fixedJoint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLOCKED);
		break;
	case Axis::Y:
		fixedJoint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLOCKED);
		break;
	case Axis::Z:
		fixedJoint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLOCKED);
		break;
	case Axis::XY:
		fixedJoint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLOCKED);
		fixedJoint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLOCKED);
		break;
	case Axis::XZ:
		fixedJoint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLOCKED);
		fixedJoint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLOCKED);
		break;
	case Axis::YZ:
		fixedJoint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLOCKED);
		fixedJoint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLOCKED);
		break;
	case Axis::XYZ:
		fixedJoint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLOCKED);
		fixedJoint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLOCKED);
		fixedJoint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLOCKED);
		break;
	default: ;
	}
}

void PM3D_API::Rigidbody::SetRotationLock(Axis axis)
{

}

void PM3D_API::Rigidbody::DrawDebugInfo() const
{
	if (isStatic)
	{
		ImGui::Text("Static!");
	} else
	{
		const physx::PxRigidDynamic* dynamic = static_cast<physx::PxRigidDynamic*>(actor);
		
		ImGui::Text("Linear vel.");
		ImGui::SameLine(100.0f); ImGui::Text("X: %f", dynamic->getLinearVelocity().x);
		ImGui::SameLine(200.0f); ImGui::Text("Y: %f", dynamic->getLinearVelocity().y);
		ImGui::SameLine(300.0f); ImGui::Text("Z: %f", dynamic->getLinearVelocity().z);

		ImGui::Text("Angular vel.");
		ImGui::SameLine(100.0f); ImGui::Text("X: %f", dynamic->getAngularVelocity().x);
		ImGui::SameLine(200.0f); ImGui::Text("Y: %f", dynamic->getAngularVelocity().y);
		ImGui::SameLine(300.0f); ImGui::Text("Z: %f", dynamic->getAngularVelocity().z);
	}
}
