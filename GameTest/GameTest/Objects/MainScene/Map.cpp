#include "Map.h"

#include "Api/Public/Component/Basic/Physics/InstancedMeshCollider.h"
#include "Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Component/Basic/Render/3D/InstancedMeshRenderer.h"
#include "Api/Public/Shader/Basic/DefaultShaderInstanced.h"
#include "Api/Public/Util/FilterGroup.h"
#include "GameTest/Components/SnowRenderer.h"

Map::Map() : GameObject("World"), mapImporter("Map/map_data.json")
{
}

void Map::Initialize()
{
    /*
    auto meshRenderer = std::make_unique<PM3D_API::MeshRenderer>(
        std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx"),
        "Map/map.obj"
    );
    const auto meshRendererPtr = meshRenderer.get();
    AddComponent(std::move(meshRenderer));
    meshRendererPtr->Initialize();
    */
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
        parent->GetScene()->GetPhysicsResolver()->GetDefaultMaterial(),
        meshRendererPtr->getChargeur()
    );
    const auto meshColliderPtr = meshCollider.get();
    AddComponent(std::move(meshCollider));
    meshColliderPtr->Initialize();

    if (!mapImporter.IsParseSuccessful()) return;

    for (const auto& [name, chargeur, objects] : mapImporter.GetParsedObjects())
    {
        if (!chargeur)
        {
            std::cerr << "Map::Initialize() : chargeur is null" << std::endl;
        }

        auto shader = std::make_unique<PM3D_API::DefaultShaderInstanced>(L"shader/NewShaderInstanced.fx");

        auto gameObject = std::make_unique<GameObject>("MapObject-" + name);
        auto renderer = std::make_unique<PM3D_API::InstancedMeshRenderer>(
            std::move(shader),
            chargeur,
            std::move(objects)
        );
        const auto rendererPtr = renderer.get();
        const auto gameObjectPtr = gameObject.get();

        AddChild(std::move(gameObject));
        gameObjectPtr->Initialize();

        gameObjectPtr->AddComponent(std::move(renderer));
        rendererPtr->Initialize();

        auto instancedMeshCollider = std::make_unique<PM3D_API::InstancedMeshCollider>(
            scene->GetPhysicsResolver()->GetDefaultMaterial());
        const auto instancedMeshColliderPtr = instancedMeshCollider.get();
        gameObjectPtr->AddComponent(std::move(instancedMeshCollider));
        instancedMeshColliderPtr->Initialize();
    }

    // =========================================
    // LANDSCAPE 0
    // =========================================

    auto landscapeWithTunnel = std::make_unique<GameObject>(
        "LandscapeWithTunnel"
    );
    auto landscapeWithTunnelMeshRenderer = std::make_unique<PM3D_API::MeshRenderer>(
        std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx"),
        "Map/landscape0.obj"
    );

    const auto landscapeWithTunnelPtr = landscapeWithTunnel.get();
    AddChild(std::move(landscapeWithTunnel));

    const auto landscapeWithTunnelMeshRendererPtr = landscapeWithTunnelMeshRenderer.get();
    landscapeWithTunnelPtr->AddComponent(std::move(landscapeWithTunnelMeshRenderer));

    landscapeWithTunnelPtr->Initialize();
    landscapeWithTunnelMeshRendererPtr->Initialize();

    // =========================================
    // LANDSCAPE 1
    // =========================================

    auto landscape1 = std::make_unique<GameObject>(
        "Landscape1"
    );
    auto landscape1MeshRenderer = std::make_unique<PM3D_API::MeshRenderer>(
        std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx"),
        "Map/landscape1.obj"
    );

    const auto landscape1Ptr = landscape1.get();
    AddChild(std::move(landscape1));

    const auto landscape1MeshRendererPtr = landscape1MeshRenderer.get();
    landscape1Ptr->AddComponent(std::move(landscape1MeshRenderer));

    landscape1Ptr->Initialize();
    landscape1MeshRendererPtr->Initialize();

    // =========================================
    // LANDSCAPE 2
    // =========================================

    auto landscape2 = std::make_unique<GameObject>(
        "Landscape2"
    );
    auto landscape2MeshRenderer = std::make_unique<PM3D_API::MeshRenderer>(
        std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx"),
        "Map/landscape2.obj"
    );

    const auto landscape2Ptr = landscape2.get();
    AddChild(std::move(landscape2));

    const auto landscape2MeshRendererPtr = landscape2MeshRenderer.get();
    landscape2Ptr->AddComponent(std::move(landscape2MeshRenderer));

    landscape2Ptr->Initialize();
    landscape2MeshRendererPtr->Initialize();

    // =========================================
    // TUNNEL
    // =========================================

    auto tunnelObj = std::make_unique<GameObject>(
        "Tunnel"
    );
    const auto tunnelObjPtr = tunnelObj.get();
    AddChild(std::move(tunnelObj));

    auto tunnelRenderer = std::make_unique<PM3D_API::MeshRenderer>(
        std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx"),
        "Map/tunnel.obj"
    );
    const auto tunnelRendererPtr = tunnelRenderer.get();
    tunnelObjPtr->AddComponent(std::move(tunnelRenderer));
    tunnelRendererPtr->Initialize();

    auto tunnelRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto tunnelRigidbodyPtr = tunnelRigidbody.get();
    tunnelObjPtr->AddComponent(std::move(tunnelRigidbody));
    tunnelRigidbodyPtr->Initialize();

    auto tunnelMeshCollider = std::make_unique<PM3D_API::MeshCollider>(
        parent->GetScene()->GetPhysicsResolver()->GetDefaultMaterial()
    );
    const auto tunnelMeshColliderPtr = tunnelMeshCollider.get();
    tunnelObjPtr->AddComponent(std::move(tunnelMeshCollider));
    tunnelMeshColliderPtr->Initialize();

    physx::PxFilterData filterDataTunnel;
    filterDataTunnel.word0 = FilterGroup::eTUNNEL;
    physx::PxShape* tunnelShape = tunnelMeshColliderPtr->getShape();
    tunnelShape->setSimulationFilterData(filterDataTunnel);
}
