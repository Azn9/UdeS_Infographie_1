#include "Core/Public/Core/dispositifd3d11.h"
#include "Core/Public/Texture/Texture.h"
#include "Core/Public/Util/resource.h"
#include "Core/Public/Util/util.h"
#include "Core/Public/Texture/DDSTextureLoader.h"

using namespace DirectX;

namespace PM3D
{
CTexture::CTexture(const std::wstring& filename,
                   CDispositifD3D11*
                   pDispositif)
	: m_Filename(filename)
	  , m_Texture(nullptr)
{
	ID3D11Device* pDevice = pDispositif->GetD3DDevice();
	// Charger la texture en ressource
	DXEssayer(CreateDDSTextureFromFile(pDevice,
		m_Filename.c_str(),
		nullptr,
		&m_Texture), DXE_FICHIERTEXTUREINTROUVABLE);
}

CTexture::~CTexture()
{
	DXRelacher(m_Texture);
}

}
