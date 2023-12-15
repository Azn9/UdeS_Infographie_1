#include "Sphere.h"

#include "SphereRenderer.h"
#include "SphereShader.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Component/Basic/Physics/SphereCollider.h"
#include "Api/Public/Util/FilterGroup.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Components/MovableComponent.h"
#include "GameTest/Components/SizeModifierComponent.h"
#include "GameTest/Components/WalkSoundComponent.h"

void Sphere::Initialize()
{
    SetWorldScale(XMFLOAT3(.2f, .2f, .2f));
    SetWorldPosition(XMFLOAT3(6.f, -50.f, 66.f));

    auto sphereShader = std::make_unique<SphereShader>(L"shader/SphereShader.fx");
    auto sphereRenderer = std::make_unique<SphereRenderer>(std::move(sphereShader));
    const auto sphereRendererPtr = sphereRenderer.get();
    AddComponent(std::move(sphereRenderer));
    sphereRendererPtr->Initialize();

    auto sphereRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto sphereRigidbodyPtr = sphereRigidbody.get();
    AddComponent(std::move(sphereRigidbody));
    sphereRigidbodyPtr->Initialize();

    static_cast<physx::PxRigidDynamic*>(sphereRigidbodyPtr->GetActor())->setMass(20.f);

    auto sphereCollider = std::make_unique<PM3D_API::SphereCollider>(PxGetPhysics().createMaterial(0.4f, 0.4f, 0.f));
    const auto sphereColliderPtr = sphereCollider.get();
    AddComponent(std::move(sphereCollider));
    sphereColliderPtr->Initialize();
    physx::PxFilterData filterDataSnowball;
    filterDataSnowball.word0 = FilterGroup::eSNOWBALL;
    physx::PxShape* sphereShape = sphereColliderPtr->getShape();
    sphereShape->setSimulationFilterData(filterDataSnowball);

    scene->GetMainCamera()->GetComponent<CameraFollowComponent>()->SetObjectToFollow(this);

    AddComponent(std::make_unique<SizeModifierComponent>());
    AddComponent(std::make_unique<MovableComponent>());

    auto walkSoundComponent = std::make_unique<WalkSoundComponent>();
    const auto walkSoundComponentPtr = walkSoundComponent.get();
    AddComponent(std::move(walkSoundComponent));
    walkSoundComponentPtr->Initialize();
}
