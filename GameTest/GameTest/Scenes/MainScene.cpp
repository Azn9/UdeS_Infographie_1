#include "MainScene.h"

#include <memory>

#include "../../PM3DApi/Api/Public/Camera/Camera.h"

#include "../../PM3DApi/Api/Public/Component/Basic/Physics/SphereCollider.h"
#include "../../PM3DApi/Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "../../PM3DApi/Api/Public/Component/Basic/Physics/Rigidbody.h"

#include "../../PM3DApi/Api/Public/Component/Basic/Render/MeshRenderer.h"
#include "../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../PM3DApi/Api/Public/GameObject/Basic/BasicSphere.h"
#include "../../PM3DApi/Api/Public/Light/AmbiantLight.h"
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
#include "../../../PM3DApi/Api/Public/Camera/Camera.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/3D/MeshRenderer.h"
#include "../../../PM3DApi/Api/Public/EventSystem/EventSystem.h"
#include "../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../PM3DApi/Api/Public/Light/AmbiantLight.h"
#include "GameTest/TimeScaleTest.h"
#include "GameTest/Components/CameraMoverComponent.h"
#include "GameTest/Components/CameraFollowComponent.h"
#include "GameTest/Components/MovableComponent.h"
#include "GameTest/UI/TestUIObject.h"
#include "GameTest/Components/SizeModifierComponent.h"
#include "../TimeScaleTest.h"
#include "../Components/CameraMoverComponent.h"
#include "GameTest/Heightmap.h"
#include "GameTest/Pine.h"

#include "Component/Basic/Physics/MeshCollider.h"
#include "Component/Basic/Physics/SphereCollider.h"
#include "GameObject/Basic/BasicSphere.h"

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
        XMVectorSet(0.0f, -5.0f, 15.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
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
    /*
    // ============= Add a plane =============
    auto map = std::make_unique<Heightmap>();
    map->SetWorldPosition(XMFLOAT3(0.0f, -10.0f, 10.0f));
    map->SetWorldScale(XMFLOAT3(10.f, 10.f, 10.f));
    map->Initialize();
    const auto mapPtr = map.get();
    AddChild(std::move(map));

    auto mapRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto mapRigidbodyPtr = mapRigidbody.get();
    mapPtr->AddComponent(std::move(mapRigidbody));
    mapRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(physicsResolver->GetDefaultMaterial());
    const auto meshColliderPtr = meshCollider.get();
    mapPtr->AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();

    // ============= Add a sphere =============
    auto sphere = std::make_unique<PM3D_API::BasicSphere>("Sphere");
    sphere->SetWorldPosition(XMFLOAT3(0.0f, -9.5f, 10.0f));
    sphere->SetWorldScale(XMFLOAT3(.2f, .2f, .2f));
    sphere->Initialize();
    const auto spherePtr = sphere.get();
    AddChild(std::move(sphere));

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
    
    GetMainCamera()->GetComponent<CameraFollowComponent>()->SetObjectToFollow(sphere.get());

    sphere->AddComponent(std::make_unique<SizeModifierComponent>());

    sphere->AddComponent(std::make_unique<MovableComponent>());

    AddChild(std::move(sphere));

    

    PM3D_API::GameHost::GetInstance()->AddDebugRenderer(std::move(std::make_unique<TimeScaleTest>()));
    PM3D::Time::GetInstance().SetTimeScale(0.0f);
    */

    // ============= Add a pine =============
    auto pine = std::make_unique<Pine>();
    pine->SetWorldPosition(XMFLOAT3(-0.4f, -12.0f, 14.0f));
    pine->SetWorldScale(XMFLOAT3(1.5f, 1.5f, 1.5f));
    pine->SetWorldRotation(XMFLOAT3(0.0f, 0.90f, 0.f));
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

void MainScene::InitializeUI()
{
    Scene::InitializeUI(); // Init the base canvas

    auto testUiObject = std::make_unique<TestUIObject>(
        "TestUIObject",
        XMFLOAT2(0.5f, 0.5f),
        XMFLOAT2(0.25f, 0.25f),
        true,
        true
    );
    testUiObject->Initialize();
    AddUiChild(std::move(testUiObject));
}
