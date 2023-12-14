#include "DITScene.h"

#include "Api/Public/Component/Basic/Render/3D/InstancedMeshRenderer.h"
#include "Api/Public/Shader/Basic/DefaultShaderInstanced.h"
#include "Api/Public/Util/MapImporter/MapImporter.h"
#include "Core/Public/Mesh/FastobjChargeur.h"
#include "GameTest/Objects/LoadingScene/CustomPlane.h"

using namespace DirectX;

void DITScene::InitializeCamera()
{
    auto mainCamera = std::make_unique<PM3D_API::Camera>(
        "Main camera",
        PM3D_API::Camera::PERSPECTIVE,
        XMFLOAT3(0.0f, 8.0f, -15.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
    );
    mainCamera->SetFieldOfView(45.0f);
    mainCamera->SetFarDist(1000.0f);
    //mainCamera->AddComponent(std::make_unique<CameraMoverComponent>());
    mainCamera->SetClearColor(XMFLOAT3(216.f / 255.f, 242.f / 255.f, 255.f / 255.f));
    SetMainCamera(std::move(mainCamera));
}

void DITScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        XMFLOAT3(-1.0f, -1.0f, 0.0f)
    );
    directionalLight->SetIntensity(1.0f);
    directionalLight->Initialize();
    AddLight(std::move(directionalLight));
}

void DITScene::InitializeObjects()
{
    // ============= Add a plane =============
    auto plane = std::make_unique<CustomPlane>();
    plane->SetWorldScale(XMFLOAT3(100.0f, 1.0f, 100.0f));
    plane->SetWorldScale(XMFLOAT3(10.0f, 1.0f, 10.0f));
    plane->Initialize();
    AddChild(std::move(plane));

    // ============= Add some objects =============
    auto parent = std::make_unique<PM3D_API::GameObject>("Parent");
    const auto parentPtr = parent.get();
    AddChild(std::move(parent));

    const auto chargeur = new PM3D::FastobjChargeur;
    PM3D::CParametresChargement params;
    params.NomChemin = "";
    params.NomFichier = "Map/obj/tree_pineDefaultA.obj";

    chargeur->Chargement(params);

    auto instances = std::vector<PM3D_API::MapImporter::InstanceObject>{};
    instances.push_back(PM3D_API::MapImporter::InstanceObject(
        XMFLOAT3(0.0f, 0.0f, 0.0f),
        XMFLOAT3(0.0f, 0.0f, 0.0f),
        XMFLOAT3(10.0f, 10.0f, 10.0f)
    ));
    instances.push_back(PM3D_API::MapImporter::InstanceObject(
        XMFLOAT3(-5.0f, 0.0f, 0.0f),
        XMFLOAT3(0.0f, 0.0f, 0.0f),
        XMFLOAT3(5.0f, 5.0f, 5.0f)
    ));

    auto shader = std::make_unique<PM3D_API::DefaultShaderInstanced>(L"shader/NewShaderInstanced.fx");
    auto instancedMeshRenderer = std::make_unique<PM3D_API::InstancedMeshRenderer>(
        std::move(shader),
        chargeur,
        std::move(instances)
    );
    instancedMeshRenderer->Initialize();

    parentPtr->AddComponent(std::move(instancedMeshRenderer));
}
