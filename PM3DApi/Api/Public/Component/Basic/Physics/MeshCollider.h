#pragma once

#include "Collider.h"
#include "Core/Public/Mesh/chargeur.h"
#include "Core/Public/Mesh/FastobjChargeur.h"

namespace PM3D
{
    class FastobjChargeur;
}

namespace PM3D_API
{
    class MeshCollider : public Collider
    {
    public:
        explicit MeshCollider(
            physx::PxMaterial* _material,
            PM3D::IChargeur* _chargeur = nullptr
        ) : Collider(_material), shape(nullptr), chargeur(_chargeur)
        {
        }

        explicit MeshCollider(
            physx::PxMaterial* _material,
            const std::string& meshName
        ) : Collider(_material), shape(nullptr)
        {
            chargeur = new PM3D::FastobjChargeur();

            PM3D::CParametresChargement parametres;
            parametres.NomChemin = "";
            parametres.NomFichier = meshName;

            chargeur->Chargement(parametres);
        }

        void Initialize() override;

        physx::PxShape* getShape() { return shape; }

    protected:
        physx::PxShape* shape;

    private:
        PM3D::IChargeur* chargeur;
    };
}
