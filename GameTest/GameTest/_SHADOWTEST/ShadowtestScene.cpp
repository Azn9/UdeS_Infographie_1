#include "ShadowtestScene.h"

#include "Api/Private/Light/Shadow/ShadowProcessor.h"
#include "Api/Public/GameObject/Basic/BasicCube.h"
#include "Api/Public/GameObject/Basic/BasicPlane.h"
#include "GameTest/Components/LoadingScene/CameraMoverComponent.h"

void ShadowtestScene::InitializeCamera()
{
    auto camera = std::make_unique<PM3D_API::Camera>(
        "Camera",
        PM3D_API::Camera::CameraType::PERSPECTIVE,
        DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    camera->SetClearColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
    camera->SetFieldOfView(45.f);
    camera->SetNearDist(0.1f);
    camera->SetFarDist(100.f);

    camera->AddComponent(std::make_unique<CameraMoverComponent>());

    SetMainCamera(std::move(camera));
}

void ShadowtestScene::InitializeLights()
{
    auto directionalLight = std::make_unique<PM3D_API::DirectionalLight>(
        "Directional light",
        XMFLOAT3(-1.0f, -1.0f, 0.f) // TODO : normalize
    );
    const auto directionalLightPtr = directionalLight.get();
    AddLight(std::move(directionalLight));

    directionalLightPtr->SetIntensity(1.0f);
    directionalLightPtr->Initialize();
}

void ShadowtestScene::InitializeObjects()
{
    auto cube = std::make_unique<PM3D_API::BasicCube>(
        "Cube",
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3(10.0f, 0.1f, 10.0f)
    );
    cube->Initialize();
    AddChild(std::move(cube));

    auto tree1 = std::make_unique<GameObject>("tree");
    tree1->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree1->Initialize();

    auto tree2 = std::make_unique<GameObject>("tree");
    tree2->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree2->SetWorldPosition({-1.5f, 0.f, -1.5f});
    tree2->SetWorldRotation({45.f, 0.f, 45.f});
    tree2->Initialize();

    auto tree3 = std::make_unique<GameObject>("tree");
    tree3->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree3->SetWorldPosition({6.f, 0.f, 6.f});
    tree3->Initialize();

    auto tree4 = std::make_unique<GameObject>("tree");
    tree4->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree4->SetWorldPosition({2.8f, 0.f, 1.5f});
    tree4->Initialize();
    auto tree5 = std::make_unique<GameObject>("tree");
    tree5->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree5->SetWorldPosition({3.5f, 0.f, -1.7f});
    tree5->Initialize();
    auto tree6 = std::make_unique<GameObject>("tree");
    tree6->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree6->SetWorldPosition({5.f, 0.f, 0.f});
    tree6->Initialize();
    auto tree7 = std::make_unique<GameObject>("tree");
    tree7->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree7->SetWorldPosition({-0.6f, 0.f, 3.f});
    tree7->Initialize();
    auto tree8 = std::make_unique<GameObject>("tree");
    tree8->SetWorldScale({5.0f, 5.0f, 5.0f});
    tree7->SetWorldPosition({-0.5f, 0.f, -3.f});
    tree8->Initialize();

    auto shader1 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree1->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader1), "tree_pineTallA.obj"));
    auto shader2 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree2->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader2), "tree_pineTallA.obj"));
    auto shader3 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree3->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader3), "tree_pineTallA.obj"));
    auto shader4 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree4->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader4), "tree_pineTallA.obj"));
    auto shader5 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree5->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader5), "tree_pineTallA.obj"));
    auto shader6 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree6->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader6), "tree_pineTallA.obj"));
    auto shader7 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree7->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader7), "tree_pineTallA.obj"));
    auto shader8 = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
    tree8->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader8), "tree_pineTallA.obj"));

    AddChild(std::move(tree1));
    AddChild(std::move(tree2));
    AddChild(std::move(tree3));
    AddChild(std::move(tree4));
    AddChild(std::move(tree5));
    AddChild(std::move(tree6));
    AddChild(std::move(tree7));
    AddChild(std::move(tree8));

    auto shadowProcessor = std::make_unique<ShadowProcessor>();
    shadowProcessor->Initialize();
    shadowProcessor->SetScene(this);
    AddComponent(std::move(shadowProcessor));
}
