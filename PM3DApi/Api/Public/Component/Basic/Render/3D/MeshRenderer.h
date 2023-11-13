#pragma once

#include <string>

#include "../../../../Util/fast_obj.h"

#include "Renderer.h"
#include "Core/Imgui/imgui.h"
#include "Core/Public/Mesh/chargeur.h"
#include "Core/Public/Texture/CMaterial.h"
#include "Core/Public/Util/Time.h"
#include "Api/Public/Shader/Shader.h"

namespace PM3D_API
{
class MeshRenderer : public Renderer
{
public:
	MeshRenderer(std::unique_ptr<Shader>&& shader, std::string meshName);
	MeshRenderer(std::unique_ptr<Shader>&& shader, std::shared_ptr<PM3D::IChargeur> chargeur);

	~MeshRenderer() override;

	void Initialize() override;
	void DrawSelf() const override;

	void DrawDebugInfo() const override
	{
		ImGui::Text("Visible");
		ImGui::SameLine(200.0f); ImGui::Text(IsVisible() ? "Yes" : "No");
		
		ImGui::Text("MeshLoaded");
		ImGui::SameLine(200.0f); ImGui::Text(meshLoaded ? "true" : "false");

		ImGui::Text("NombreSubmesh");
		ImGui::SameLine(200.0f); ImGui::Text(std::to_string(NombreSubmesh).c_str());

		ImGui::Text("SubmeshMaterialIndex");
		ImGui::SameLine(200.0f); ImGui::Text(std::to_string(SubmeshMaterialIndex.size()).c_str());

		ImGui::Text("SubmeshIndex");
		ImGui::SameLine(200.0f); ImGui::Text(std::to_string(SubmeshIndex.size()).c_str());

		ImGui::Text("Material");
		ImGui::SameLine(200.0f); ImGui::Text(std::to_string(Material.size()).c_str());
	}

	bool IsVisible() const override;

	std::shared_ptr<PM3D::IChargeur> getChargeur() { return chargeur; }

protected:
	void LoadMesh();

	std::shared_ptr<PM3D::IChargeur> chargeur;
	fastObjMesh* mesh;
	bool meshLoaded;

	// Les sous-objets
	size_t NombreSubmesh; // Nombre de sous-objets dans le mesh
	std::vector<int> SubmeshMaterialIndex; // Index des matériaux
	std::vector<int> SubmeshIndex; // Index des sous-objets
	std::vector<CMaterial> Material; // Vecteur des matériaux

	///radius of the bounding sphere
	float boundingRadius = 0.f;
};
}
