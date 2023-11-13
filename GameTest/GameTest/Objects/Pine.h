#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"
#include "Core/Public/Mesh/FastobjChargeur.h"

class Pine : public PM3D_API::BasicShape
{
public:
    Pine() : BasicShape("pine") {
        _chargeur = nullptr;
    }
    Pine(std::shared_ptr<PM3D::FastobjChargeur> chargeur) : BasicShape("pine"), _chargeur(chargeur) {}

    void ConstructRenderer(std::unique_ptr<PM3D_API::Shader>&& shader) override
    {
        auto meshRenderer = _chargeur ? std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), _chargeur) :
            std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), GetMeshFileName());
        meshRenderer->Initialize();
        GameObject::AddComponent(std::move(meshRenderer));
    }
private:
    std::shared_ptr<PM3D::IChargeur> _chargeur;
    std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
    std::string GetMeshFileName() override { return "tree_pineTallA.obj"; }
};