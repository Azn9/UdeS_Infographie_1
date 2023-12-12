#pragma once
#include <DirectXMath.h>
#include <string>
#include "json.hpp"
#include "Core/Public/Mesh/FastobjChargeur.h"
using json = nlohmann::json;

namespace PM3D_API
{
    class MapImporter
    {
    public:
        struct InstanceObject
        {
            XMFLOAT3 position;
            XMFLOAT3 rotation;
            XMFLOAT3 scale;

            explicit InstanceObject(const json& j)
            {
                const auto& position = j["position"];
                this->position = {
                    position["x"].get<float>(),
                    position["y"].get<float>(),
                    position["z"].get<float>()
                };

                const auto& rotation = j["rotation"];
                this->rotation = {
                    rotation["x"].get<float>(),
                    rotation["y"].get<float>(),
                    rotation["z"].get<float>()
                };

                const auto& scale = j["scale"];
                this->scale = {
                    scale["x"].get<float>(),
                    scale["y"].get<float>(),
                    scale["z"].get<float>()
                };
            }
        };

        struct InstancedObjectMap
        {
            std::string name;
            PM3D::FastobjChargeur* chargeur = nullptr;
            std::vector<InstanceObject> objects;
        };

        explicit MapImporter(const std::string& filename);
        ~MapImporter() = default;

        [[nodiscard]] const std::vector<InstancedObjectMap>& GetParsedObjects() const { return parsedObjects; }
        [[nodiscard]] const bool& IsParseSuccessful() const { return parseSuccessful; }

    private:
        std::vector<InstancedObjectMap> parsedObjects;
        bool parseSuccessful = false;
    };
}
