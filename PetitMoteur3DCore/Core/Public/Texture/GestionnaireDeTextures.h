#pragma once

#include <memory>
#include <vector>
#include "texture.h"

namespace PM3D
{
    class CDispositifD3D11;

    class CGestionnaireDeTextures
    {
    public:
        CTexture* GetNewTexture(
            const std::wstring& filename,
            CDispositifD3D11* pDispositif
        );

        CTexture* GetNewTextureArray(
            const std::wstring& name,
            const std::vector<std::wstring>& filenames,
            CDispositifD3D11* pDispositif
        );

        CTexture* GetTexture(const std::wstring& filename) const;

    private:
        // Le tableau de textures
        std::vector<std::unique_ptr<CTexture>> ListeTextures;
    };
}
