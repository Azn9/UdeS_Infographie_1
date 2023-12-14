#include "SRORMScene.h"

#include "Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Component/Basic/Physics/SphereCollider.h"
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "Api/Public/GameObject/Basic/BasicSphere.h"
#include "Api/Public/Shader/Basic/DefaultShader.h"
#include "Api/Public/Util/FilterGroup.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Components/MovableComponent.h"
#include "GameTest/Components/SizeModifierComponent.h"
#include "GameTest/Components/SnowMover.h"
#include "GameTest/Components/SnowRenderer.h"
#include "GameTest/Shader/SnowShader.h"

void SRORMScene::InitializePhysics()
{
    auto physicsResolver = std::make_unique<PM3D_API::PhysicsResolver>();
    physicsResolver->Initialize();
    SetPhysicsResolver(std::move(physicsResolver));
}

void SRORMScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSPECTIVE,
        XMFLOAT3(-2.0f, -50.0f, 50.0f),
        XMVectorSet(-2.0f, -50.0f, 80.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
    mainCamera->AddComponent(std::make_unique<CameraFollowComponent>());
    mainCamera->SetClearColor(XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
    SetMainCamera(std::move(mainCamera));
}

void SRORMScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        XMFLOAT3(1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void SRORMScene::InitializeObjects()
{
    auto meshRenderer = std::make_unique<SnowRenderer>(
        //std::make_unique<SnowShader>(L"shader/SnowShader_tesselate.fx", true),
        std::make_unique<SnowShader>(L"shader/SnowShader.fx"),
        "Map/map.obj",
        false
    );
    const auto meshRendererPtr = meshRenderer.get();
    AddComponent(std::move(meshRenderer));
    meshRendererPtr->Initialize();

    auto mapRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto mapRigidbodyPtr = mapRigidbody.get();
    AddComponent(std::move(mapRigidbody));
    mapRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(
        this->GetPhysicsResolver()->GetDefaultMaterial(),
        meshRendererPtr->getChargeur() // TODO replace
    );
    const auto meshColliderPtr = meshCollider.get();
    AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();

    auto sphere = std::make_unique<PM3D_API::BasicSphere>("Sphere");
    spherePtr = sphere.get();
    AddChild(std::move(sphere));
    spherePtr->SetWorldScale(XMFLOAT3(.2f, .2f, .2f));
    spherePtr->SetWorldPosition(XMFLOAT3(6.f, -50.f, 66.f));
    spherePtr->Initialize();

    auto sphereRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto sphereRigidbodyPtr = sphereRigidbody.get();
    spherePtr->AddComponent(std::move(sphereRigidbody));
    sphereRigidbodyPtr->Initialize();

    auto sphereCollider = std::make_unique<PM3D_API::SphereCollider>(PxGetPhysics().createMaterial(0.4f, 0.4f, 0.f));
    const auto sphereColliderPtr = sphereCollider.get();
    spherePtr->AddComponent(std::move(sphereCollider));
    sphereColliderPtr->Initialize();
    physx::PxFilterData filterDataSnowball;
    filterDataSnowball.word0 = FilterGroup::eSNOWBALL;
    physx::PxShape* sphereShape = sphereColliderPtr->getShape();
    sphereShape->setSimulationFilterData(filterDataSnowball);

    GetMainCamera()->GetComponent<CameraFollowComponent>()->SetObjectToFollow(spherePtr);

    spherePtr->AddComponent(std::make_unique<SizeModifierComponent>());
    spherePtr->AddComponent(std::make_unique<MovableComponent>());
    spherePtr->AddComponent(std::make_unique<SnowMover>());
}
