#include "Core/Public/Core/dispositifD3D11.h"
#include "Core/Public/Texture/GestionnaireDeTextures.h"

#include <assert.h>
#include <codecvt>
#include <iostream>

namespace PM3D
{
CTexture* CGestionnaireDeTextures::GetNewTexture(
	const std::wstring& filename,
	CDispositifD3D11* pDispositif
)
{
	// Convert wstring to string
#pragma warning(push)
#pragma warning(disable: 4996)
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	const std::string filenameStr = converter.to_bytes(filename);
#pragma warning(pop)
	
	std::cout << "CGestionnaireDeTextures::GetNewTexture(" << filenameStr << ")" << std::endl;
	
	// On vérifie si la texture est déjà dans notre liste
	CTexture* pTexture = GetTexture(filename);
	// Si non, on la crée
	if (!pTexture)
	{
		auto texture = std::make_unique<CTexture>(filename, pDispositif);
		pTexture = texture.get();
		// Puis, il est ajouté à la scène
		ListeTextures.push_back(std::move(texture));
	}
	assert(pTexture);
	return pTexture;
}

CTexture* CGestionnaireDeTextures::GetNewTextureArray(const std::wstring& name,
	const std::vector<std::wstring>& filenames,
	CDispositifD3D11* pDispositif)
{
	
	// On vérifie si la texture est déjà dans notre liste
	CTexture* pTexture = GetTexture(name);
	// Si non, on la crée
	if (!pTexture)
	{
		auto texture = std::make_unique<CTexture>(name, filenames, pDispositif);
		pTexture = texture.get();
		// Puis, il est ajouté à la scène
		ListeTextures.push_back(std::move(texture));
	}
	assert(pTexture);
	return pTexture;
}

CTexture* CGestionnaireDeTextures::GetTexture(const std::wstring& filename) const
{
	CTexture* pTexture = nullptr;
	for (auto& texture : ListeTextures)
	{
		if (texture->GetName() == filename)
		{
			pTexture = texture.get();
			break;
		}
	}
	return pTexture;
}
	
}
