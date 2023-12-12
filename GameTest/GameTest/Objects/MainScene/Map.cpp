#include "Map.h"

#include "Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Component/Basic/Render/3D/InstancedMeshRenderer.h"
#include "Api/Public/Shader/Basic/DefaultShaderInstanced.h"

Map::Map() : GameObject("World"), mapImporter("Map/map_data.json")
{
}

void Map::Initialize()
{
    auto meshRenderer = std::make_unique<PM3D_API::MeshRenderer>(
        std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx"),
        "Map/map.obj"
    );
    const auto meshRendererPtr = meshRenderer.get();
    AddComponent(std::move(meshRenderer));
    meshRendererPtr->Initialize();

    auto mapRigidbody = std::make_unique<PM3D_API::Rigidbody>(true);
    const auto mapRigidbodyPtr = mapRigidbody.get();
    AddComponent(std::move(mapRigidbody));
    mapRigidbodyPtr->Initialize();

    auto meshCollider = std::make_unique<PM3D_API::MeshCollider>(
        parent->GetScene()->GetPhysicsResolver()->GetDefaultMaterial()
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
        gameObjectPtr->AddComponent(std::move(renderer));
        rendererPtr->Initialize();
        gameObjectPtr->Initialize();

        // TODO : colliders
    }
}
