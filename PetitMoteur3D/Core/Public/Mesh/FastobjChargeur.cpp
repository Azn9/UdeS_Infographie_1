#include "stdafx.h"
#include "FastobjChargeur.h"

#ifndef FAST_OBJ_IMPLEMENTATION
#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"
#endif

PM3D::FastobjChargeur::~FastobjChargeur()
{
	if (mesh != nullptr)
		fast_obj_destroy(mesh);
}

void PM3D::FastobjChargeur::Chargement(const CParametresChargement& param)
{
	const auto filePath = param.NomChemin + param.NomFichier;
	mesh = fast_obj_read(filePath.c_str());

	if (!mesh)
		throw std::runtime_error("Impossible de charger le fichier " + filePath);

	for (unsigned int i = 0; i < mesh->object_count; ++i)
	{
		SubsetIndex.push_back(static_cast<int>(mesh->objects[i].index_offset));
	}

	for (unsigned int i = 0; i < mesh->position_count; ++i)
	{
		const float x = mesh->positions[3*i + 0];
		const float y = mesh->positions[3*i + 1];
		const float z = mesh->positions[3*i + 2];

		Position.emplace_back(x, y, -z);
	}

	for (unsigned int i = 0; i < mesh->texcoord_count; ++i)
	{
		const float x = mesh->texcoords[2*i + 0];
		const float y = mesh->texcoords[2*i + 1];

		CoordTex.emplace_back(x, y);
	}

	for (unsigned int i = 0; i < mesh->normal_count; ++i)
	{
		const float x = mesh->normals[3*i + 0];
		const float y = mesh->normals[3*i + 1];
		const float z = mesh->normals[3*i + 2];

		Normale.emplace_back(x, y, -z);
	}

	for (unsigned int i = 0; i < mesh->material_count; ++i)
	{
		materialName.emplace_back(mesh->materials[i].name);
	}

	for (unsigned int i = 0; i < mesh->index_count; ++i)
	{
		tabIndex.emplace_back(i);
	}
	
	for (unsigned int i = 0; i < mesh->group_count; ++i)
	{
		const fastObjGroup& grp = mesh->groups[i];

		int idx = 0;
		for (unsigned int j = 0; j < grp.face_count; ++j)
		{
			const auto faceVertices = mesh->face_vertices[j];

			std::vector<XMFLOAT3> positions;
			std::vector<XMFLOAT2> texcoords;
			std::vector<XMFLOAT3> normals;

			for (unsigned int k = 0; k < faceVertices; ++k)
			{
				const fastObjIndex mi = mesh->indices[grp.index_offset + idx];

				indexFaces.push_back(mi.p);

				const float p1 = mesh->positions[3 * mi.p + 0];
				const float p2 = mesh->positions[3 * mi.p + 1];
				const float p3 = mesh->positions[3 * mi.p + 2];

				const float n1 = mesh->normals[3 * mi.n + 0];
				const float n2 = mesh->normals[3 * mi.n + 1];
				const float n3 = mesh->normals[3 * mi.n + 2];

				const float t1 = mesh->texcoords[2 * mi.t + 0];
				const float t2 = mesh->texcoords[2 * mi.t + 1];

				positions.emplace_back(p1, p2, -p3);
				normals.emplace_back(n1, n2, -n3);
				texcoords.emplace_back(t1, t2);

				idx++;
			}

			const auto v1 = positions[0];
			const auto v2 = positions[1];
			const auto v3 = positions[2];

			const auto n1 = normals[0];
			const auto n2 = normals[1];
			const auto n3 = normals[2];

			const auto t1 = texcoords[0];
			const auto t2 = texcoords[1];
			const auto t3 = texcoords[2];

			// delta
			auto deltaPos1 = XMFLOAT3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
			auto deltaPos2 = XMFLOAT3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
			auto deltaUV1 = XMFLOAT2(t2.x - t1.x, t2.y - t1.y);
			auto deltaUV2 = XMFLOAT2(t3.x - t1.x, t3.y - t1.y);

			// facteur d'échelle
			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			// tangente
			XMFLOAT3 tangent;
			tangent.x = f * (deltaUV2.y * deltaPos1.x - deltaUV1.y * deltaPos2.x);
			tangent.y = f * (deltaUV2.y * deltaPos1.y - deltaUV1.y * deltaPos2.y);
			tangent.z = f * (deltaUV2.y * deltaPos1.z - deltaUV1.y * deltaPos2.z);

			// binormale
			XMFLOAT3 binormal;
			binormal.x = f * (-deltaUV2.x * deltaPos1.x + deltaUV1.x * deltaPos2.x);
			binormal.y = f * (-deltaUV2.x * deltaPos1.y + deltaUV1.x * deltaPos2.y);
			binormal.z = f * (-deltaUV2.x * deltaPos1.z + deltaUV1.x * deltaPos2.z);

			BiNormale.push_back(binormal);
			BiNormale.push_back(binormal);
			BiNormale.push_back(binormal);
			Tangente.push_back(tangent);
			Tangente.push_back(tangent);
			Tangente.push_back(tangent);
		}
	}

	SubsetIndex.push_back(static_cast<int>(tabIndex.size()));
}

size_t PM3D::FastobjChargeur::GetNombreSommets() const
{
	return tabIndex.size();
}

int PM3D::FastobjChargeur::GetNombreSubset() const
{
	return mesh->object_count;
}

size_t PM3D::FastobjChargeur::GetNombreMaterial() const
{
	return mesh->material_count;
}

CMaterial PM3D::FastobjChargeur::GetMaterial(int _i) const
{
	const auto meshMaterial = mesh->materials[_i];
	CMaterial material;
	
	if (meshMaterial.map_Kd.path)
	{
		material.albedoTextureFileName = meshMaterial.map_Kd.path; // ?
	}
	else
	{
		material.albedoTextureFileName = "";
	}

	if (meshMaterial.map_bump.path)
	{
		material.normalmapTextureFileName = meshMaterial.map_bump.path;
	} else
	{
		material.normalmapTextureFileName = "";
	}
	
	material.NomMateriau = meshMaterial.name;
	
	const auto Ka = meshMaterial.Ka;
	material.Ambient = XMFLOAT4(Ka[0], Ka[1], Ka[2], 1.0f);

	const auto Kd = meshMaterial.Kd;
	material.Diffuse = XMFLOAT4(Kd[0], Kd[1], Kd[2], 1.0f);

	const auto Ks = meshMaterial.Ks;
	material.Specular = XMFLOAT4(Ks[0], Ks[1], Ks[2], 1.0f);
	
	material.Puissance = meshMaterial.Ns;

	return material;
}

const std::string& PM3D::FastobjChargeur::GetMaterialName(int i) const
{
	return materialName[i];
}

void PM3D::FastobjChargeur::CopieSubsetIndex(std::vector<int>& dest) const
{
	dest = SubsetIndex;
}

const XMFLOAT3& PM3D::FastobjChargeur::GetPosition(int NoSommet) const
{
	return Position[mesh->indices[NoSommet].p];
}

const XMFLOAT2& PM3D::FastobjChargeur::GetCoordTex(int NoSommet) const
{
	return CoordTex[mesh->indices[NoSommet].t];
}

const XMFLOAT3& PM3D::FastobjChargeur::GetNormale(int NoSommet) const
{
	return Normale[mesh->indices[NoSommet].n];
}

const XMFLOAT3& PM3D::FastobjChargeur::GetBiNormale(int NoSommet) const
{
	return BiNormale[mesh->indices[NoSommet].n];
}

const XMFLOAT3& PM3D::FastobjChargeur::GetTangent(int NoSommet) const
{
	return Tangente[mesh->indices[NoSommet].n];
}

