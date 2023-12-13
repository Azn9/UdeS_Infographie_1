#include "MapImporter.h"

#include <fstream>

#include "Core/Public/Mesh/FastobjChargeur.h"


PM3D_API::MapImporter::MapImporter(
    const std::string& filename
)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    json data = json::parse(file);

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        // Load the mesh
        auto fileName = std::string(it.key());

        if (fileName.find(".obj") == std::string::npos)
        {
            fileName += ".obj";
        }

        std::cout << "Loading " << fileName << std::endl;

        const auto chargeur = new PM3D::FastobjChargeur();
        chargeur->Chargement(fileName);

        std::vector<InstanceObject> objects;

        for (auto it2 = it.value().begin(); it2 != it.value().end(); ++it2)
        {
            objects.emplace_back(*it2);
        }

        parsedObjects.push_back({
            fileName,
            chargeur,
            objects
        });
    }

    parseSuccessful = true;
}
