#pragma once
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Shader/Basic/DefaultShaderInstanced.h"
#include "Api/Public/Util/MapImporter/MapImporter.h"
#include "Core/Public/Mesh/FastobjChargeur.h"

class SkiersMultiRenderer : public PM3D_API::GameObject
{
public:
    SkiersMultiRenderer(
        std::string partName,
        PM3D::FastobjChargeur* chargeur
    ) : GameObject(std::move(partName)), chargeur(chargeur)
    {
    }

    void Initialize() override;
    void Update() override;
    bool IsVisible(XMFLOAT3 position, XMFLOAT3 scale) const;
    void DrawSelf() const override;

    void Add(GameObject* gameObject);

private:
    PM3D::FastobjChargeur* chargeur;

    std::unique_ptr<PM3D_API::DefaultShaderInstanced> shader;
    std::vector<GameObject*> toRender;

    void LoadMesh();

    fastObjMesh* mesh;
    bool meshLoaded;

    // Les sous-objets
    size_t NombreSubmesh; // Nombre de sous-objets dans le mesh
    std::vector<int> SubmeshMaterialIndex; // Index des matériaux
    std::vector<int> SubmeshIndex; // Index des sous-objets
    std::vector<CMaterial> Material; // Vecteur des matériaux

    ///radius of the bounding sphere
    float boundingRadius = 0.f;

    std::vector<PM3D_API::MapImporter::InstanceObject> instances;

    struct DrawInstance
    {
        DirectX::XMMATRIX matWorldViewProj;
        DirectX::XMMATRIX matWorld;
    };
};
