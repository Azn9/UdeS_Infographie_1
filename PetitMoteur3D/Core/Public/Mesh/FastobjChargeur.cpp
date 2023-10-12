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

void PM3D::FastobjChargeur::GetMaterial(int _i,
	std::string& _NomFichierTexture,
	std::string& _NomMateriau,
	XMFLOAT4& _Ambient,
	XMFLOAT4& _Diffuse,
	XMFLOAT4& _Specular,
	float& _Puissance) const
{
	if (mesh->materials[_i].map_d.path)
		_NomFichierTexture = mesh->materials[_i].map_d.path; // ?
	else
	{
		_NomFichierTexture = "";
	}
	
	_NomMateriau = mesh->materials[_i].name;
	
	const auto Ka = mesh->materials[_i].Ka;
	_Ambient = XMFLOAT4(Ka[0], Ka[1], Ka[2], 1.0f);

	const auto Kd = mesh->materials[_i].Kd;
	_Diffuse = XMFLOAT4(Kd[0], Kd[1], Kd[2], 1.0f);

	const auto Ks = mesh->materials[_i].Ks;
	_Specular = XMFLOAT4(Ks[0], Ks[1], Ks[2], 1.0f);
	
	_Puissance = mesh->materials[_i].d;
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
