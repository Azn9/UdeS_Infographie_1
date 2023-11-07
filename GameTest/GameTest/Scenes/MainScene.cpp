#include "MainScene.h"

#include <memory>

#include "Camera/Camera.h"

#include "Component/Basic/Physics/BoxCollider.h"
#include "Component/Basic/Physics/SphereCollider.h"
#include "Component/Basic/Physics/PlaneCollider.h"
#include "Component/Basic/Physics/MeshCollider.h"
#include "Component/Basic/Physics/Rigidbody.h"

#include "Component/Basic/Render/MeshRenderer.h"
#include "GameObject/GameObject.h"
#include "GameObject/Basic/BasicCube.h"
#include "GameObject/Basic/BasicSphere.h"
#include "Light/AmbiantLight.h"
#include "Light/PointLight.h"
#include "Util/FilterGroup.h"

#include "Mesh/FastobjChargeur.h"

#include "GameTest/CustomPlane.h"
#include "GameTest/TimeScaleTest.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Components/LightMoverComponent.h"
#include "GameTest/Components/SizeModifierComponent.h"
#include "GameTest/Heightmap.h"
#include "GameTest/Pine.h"

void MainScene::InitializePhysics()
{
    auto physicsResolver = std::make_unique<PM3D_API::PhysicsResolver>();
    physicsResolver->Initialize();
    SetPhysicsResolver(std::move(physicsResolver));
}

void MainScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSECTIVE,
        XMFLOAT3(0.0f, 0.1f, 0.0f),
        XMVectorSet(0.0f, -5.0f, 15.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarPlane(1000.0f);
    mainCamera->AddComponent(std::make_unique<CameraFollowComponent>());
    SetMainCamera(std::move(mainCamera));
}

void MainScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        XMFLOAT3(1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void MainScene::InitializeObjects()
{
    // ============= Add a plane =============
    auto map = std::make_unique<Heightmap>();
    map->SetWorldPosition(XMFLOAT3(0.0f, -10.0f, 10.0f));
    map->SetWorldScale(XMFLOAT3(2.f, 2.f, 2.f));
    map->Initialize();

    auto mapRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto mapRigidbodyPtr = mapRigidbody.get();
    map->AddComponent(std::move(mapRigidbody));
    mapRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto meshColliderPtr = meshCollider.get();
    map->AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();

    AddChild(std::move(map));

    // ============= Add a sphere =============
    auto sphere = std::make_unique<PM3D_API::BasicSphere>("Sphere");
    sphere->SetWorldPosition(XMFLOAT3(0.0f, 15.0f, 10.0f));
    sphere->SetWorldScale(XMFLOAT3(.25f, .25f, .25f));
    sphere->Initialize();

    auto sphereRigidbody = std::make_unique<PM3D_API::Rigidbody>();
    const auto sphereRigidbodyPtr = sphereRigidbody.get();
    sphere->AddComponent(std::move(sphereRigidbody));
    sphereRigidbodyPtr->Initialize();

    auto sphereCollider = std::make_unique<PM3D_API::SphereCollider>(physicsResolver->GetDefaultMaterial());
    const auto sphereColliderPtr = sphereCollider.get();
    sphere->AddComponent(std::move(sphereCollider));
    sphereColliderPtr->Initialize();
    physx::PxFilterData filterDataSnowball;
    filterDataSnowball.word0 = FilterGroup::eSNOWBALL;
    physx::PxShape* sphereShape = sphereColliderPtr->getShape();
    sphereShape->setSimulationFilterData(filterDataSnowball);
    
    GetMainCamera()->GetComponent<CameraFollowComponent>()->SetObjectToFollow(sphere.get());

    sphere->AddComponent(std::make_unique<SizeModifierComponent>());

    AddChild(std::move(sphere));

    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));
    PM3D::Time::GetInstance().SetTimeScale(0.0f);

    // ============= Add a pine =============
    auto pine = std::make_unique<Pine>();
    pine->SetWorldPosition(XMFLOAT3(-0.4f, -11.80f, 14.0f));
    pine->SetWorldScale(XMFLOAT3(1.5f, 1.5f, 1.5f));
    pine->Initialize();


    auto pineRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto pineRigidbodyPtr = pineRigidbody.get(); 
    pine->AddComponent(std::move(pineRigidbody));
    pineRigidbodyPtr->Initialize();

    auto pineMeshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto pineMeshColliderPtr = pineMeshCollider.get();
    pine->AddComponent(std::move(pineMeshCollider));
    pineMeshColliderPtr->Initialize();
    physx::PxFilterData filterDataObstacle;
    filterDataObstacle.word0 = FilterGroup::eOBSTACLE;
    physx::PxShape* treeShape = pineMeshColliderPtr->getShape();
    treeShape->setSimulationFilterData(filterDataObstacle);

    AddChild(std::move(pine));
}
