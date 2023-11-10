#include "Core/Public/Core/dispositifd3d11.h"
#include "Core/Public/Texture/Texture.h"

#include "Api/Public/Util/Util.h"
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
        const auto res = CreateDDSTextureFromFile(pDevice,
                                                  m_Filename.c_str(),
                                                  nullptr,
                                                  &m_Texture);

        if (res != S_OK)
        {
            throw std::exception(("Impossible de charger la texture " + Util::ws2s(m_Filename) + " !").c_str());
        }
    }

    CTexture::~CTexture()
    {
        DXRelacher(m_Texture);
    }
}
