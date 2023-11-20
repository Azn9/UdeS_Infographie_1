#include "Map.h"

#include "Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"

Map::Map() : GameObject("World"), mapImporter("Map/map_data.json") 
{
	
}

void Map::Initialize()
{
	auto meshRenderer = std::make_unique<PM3D_API::MeshRenderer>(
		std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx"), 
		"Map/terrain.obj"
	);
	meshRenderer->Initialize();
	AddComponent(std::move(meshRenderer));
	
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

	int treeId = 0;
	for (const auto& [name, chargeur, objects] : mapImporter.GetParsedObjects())
	{
		// TODO : instanced
		for (const auto & object : objects)
		{
			const auto tree = std::make_unique<GameObject>(
				"Tree-" + std::to_string(treeId++),
				object.position,
				object.rotation,
				object.scale
			);
			
			auto shader = std::make_unique<PM3D_API::DefaultShader>(L"shader/NewShader.fx");
			tree->AddComponent(std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), chargeur));
			tree->Initialize();
		}
	}
}
