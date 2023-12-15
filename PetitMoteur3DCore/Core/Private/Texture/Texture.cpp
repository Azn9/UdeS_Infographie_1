#include "Core/Public/Core/dispositifd3d11.h"
#include "Core/Public/Texture/Texture.h"

#include "Api/Public/Util/Util.h"
#include "Core/Public/Util/resource.h"
#include "Core/Public/Util/util.h"
#include "Core/Public/Texture/DDSTextureLoader.h"
#include <vector>
using namespace DirectX;

namespace PM3D
{
    CTexture::CTexture(const std::wstring& filename, CDispositifD3D11* pDispositif)
        : m_Name(filename), m_TextureSRV(nullptr)
    {
        ID3D11Device* pDevice = pDispositif->GetD3DDevice();

        // Charger la texture en ressource
        const auto res = CreateDDSTextureFromFile(pDevice,
                                                  m_Name.c_str(),
                                                  nullptr,
                                                  &m_TextureSRV);

        if (res != S_OK)
        {
            throw std::exception(("Impossible de charger la texture " + Util::ws2s(m_Name) + " !").c_str());
        }
    }

    //Loading texture array
    CTexture::CTexture(const std::wstring& name, const std::vector<std::wstring>& filenames, CDispositifD3D11* pDispositif)
        : m_Name(name)
    {
        if(filenames.empty())
        {
            throw std::exception("Aucun fichiers fournis à CTexture::CTexture");
        }
        
        ID3D11Device* pDevice = pDispositif->GetD3DDevice();

        //all texture creation
        std::vector<ID3D11Texture2D*> textures{};
        for (const std::wstring& filename : filenames)
        {
            
            ID3D11Resource* ressource = nullptr;
            const auto res = CreateDDSTextureFromFile(pDevice,
                                                  filename.c_str(),
                                                  &ressource,
                                                  nullptr);
            if (res != S_OK)
            {
                throw std::exception(("Impossible de charger la texture " + Util::ws2s(filename) + " !").c_str());
            }

            textures.push_back(static_cast<ID3D11Texture2D*>(ressource));
        }

        D3D11_TEXTURE2D_DESC desc;
        textures[0]->GetDesc(&desc);

        //Texture array creation
        D3D11_TEXTURE2D_DESC textureArrayDesc = {};
        textureArrayDesc.Width = desc.Width;
        textureArrayDesc.Height = desc.Height;
        textureArrayDesc.MipLevels = desc.MipLevels;
        textureArrayDesc.ArraySize = static_cast<UINT>(textures.size());
        textureArrayDesc.Format = desc.Format;
        textureArrayDesc.SampleDesc.Count = 1;
        textureArrayDesc.SampleDesc.Quality = 0;
        textureArrayDesc.Usage = D3D11_USAGE_DEFAULT;
        textureArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureArrayDesc.CPUAccessFlags = 0;
        textureArrayDesc.MiscFlags = 0;

        ID3D11Texture2D* textureArray = nullptr;
        DXEssayer(pDevice->CreateTexture2D(&textureArrayDesc, nullptr, &textureArray));

        //Filling the array
        for (UINT i = 0; i < textures.size(); ++i) {
            pDispositif->GetImmediateContext()->CopySubresourceRegion(
                textureArray, i * desc.MipLevels,
                0, 0, 0, textures[i], 0, nullptr);
        }

        //SRV creation
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = desc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        srvDesc.Texture2DArray.MipLevels = desc.MipLevels;
        srvDesc.Texture2DArray.ArraySize = static_cast<UINT>(textures.size());

        DXEssayer(pDevice->CreateShaderResourceView(textureArray, &srvDesc, &m_TextureSRV));//essayer avec desc = null

        for (ID3D11Texture2D* tex : textures) {
            tex->Release();
        }
        textureArray->Release();
    }

    CTexture::~CTexture()
    {
        DXRelacher(m_TextureSRV);
    }
}
