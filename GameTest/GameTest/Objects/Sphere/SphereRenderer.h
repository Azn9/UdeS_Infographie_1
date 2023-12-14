#pragma once
#include "Api/Public/Component/Basic/Render/3D/Renderer.h"
#include "Core/Public/Mesh/chargeur.h"
#include "Core/Public/Mesh/fast_obj.h"

class SphereRenderer final : public PM3D_API::Renderer
{
public:
    explicit SphereRenderer(std::unique_ptr<PM3D_API::Shader>&& shader) : Renderer(std::move(shader)), NombreSubmesh(0)
    {
    }

    ~SphereRenderer() override;
    void Initialize() override;
    void DrawSelf() const override;

protected:
    void LoadMesh();

    PM3D::IChargeur* chargeur = nullptr;
    fastObjMesh* mesh = nullptr;
    bool meshLoaded = false;

    // Les sous-objets
    size_t NombreSubmesh; // Nombre de sous-objets dans le mesh
    std::vector<int> SubmeshMaterialIndex; // Index des matériaux
    std::vector<int> SubmeshIndex; // Index des sous-objets
    std::vector<CMaterial> Material; // Vecteur des matériaux

    ///radius of the bounding sphere
    float boundingRadius = 0.f;
};
