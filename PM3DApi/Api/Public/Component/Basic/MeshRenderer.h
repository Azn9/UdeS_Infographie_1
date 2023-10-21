#pragma once

#include "../../Component/Component.h"

#include <string>

#include "../../Util/fast_obj.h"

#include "Renderer.h"
#include "../../../../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../../../../PetitMoteur3D/Core/Public/Mesh/chargeur.h"
#include "../../../../../PetitMoteur3D/Core/Public/Texture/CMaterial.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/Time.h"

namespace PM3D_API
{
class MeshRenderer final : public Renderer
{
public:
	MeshRenderer(std::unique_ptr<Shader>&& shader, std::string meshName);
	MeshRenderer(std::unique_ptr<Shader>&& shader, PM3D::IChargeur* chargeur);

	~MeshRenderer() override;

	void Initialize() override;
	void DrawSelf() const override;

	void DrawDebugInfo() const override
	{
		ImGui::Text("Draw time");

		ImGui::Text("A");
		ImGui::SameLine(100.0f);
		ImGui::Text((std::to_string(Time::GetTimeIntervalsInSec(start, aTime) * 1000.0) + " ms").c_str());

		ImGui::Text("B");
		ImGui::SameLine(100.0f);
		ImGui::Text((std::to_string(Time::GetTimeIntervalsInSec(aTime, bTime) * 1000.0) + " ms").c_str());

		ImGui::Text("C");
		ImGui::SameLine(100.0f);
		ImGui::Text((std::to_string(Time::GetTimeIntervalsInSec(bTime, cTime) * 1000.0) + " ms").c_str());

		ImGui::Text("D");
		ImGui::SameLine(100.0f);
		ImGui::Text((std::to_string(Time::GetTimeIntervalsInSec(cTime, dTime) * 1000.0) + " ms").c_str());
	}
private:
	void LoadMesh();

	PM3D::IChargeur* chargeur;
	fastObjMesh* mesh;
	bool meshLoaded;

	// Les sous-objets
	int NombreSubmesh; // Nombre de sous-objets dans le mesh
	std::vector<int> SubmeshMaterialIndex; // Index des matériaux
	std::vector<int> SubmeshIndex; // Index des sous-objets
	std::vector<CMaterial> Material; // Vecteur des matériaux

	mutable uint64_t start;
	mutable uint64_t aTime;
	mutable uint64_t bTime;
	mutable uint64_t cTime;
	mutable uint64_t dTime;
	
};
}
