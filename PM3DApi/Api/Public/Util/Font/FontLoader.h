#pragma once
#include <map>
#include <string>

#include "Core/Public/Texture/texture.h"

namespace PM3D_API
{
    struct CharData
    {
        char c;
        int x;
        int y;
        int width;
        int height;
        int xoffset;
        int yoffset;
        int xadvance;
    };

    class FontLoader
    {
    public:
        FontLoader(): texture(nullptr)
        {
        }

        explicit FontLoader(const std::string& fntFileName, const std::wstring& ddsFile);

        bool HasData(const char c) const { return charDatas.contains(c); }
        [[nodiscard]] CharData GetCharData(const char c) const { return charDatas.at(c); }
        [[nodiscard]] PM3D::CTexture* GetTexture() const { return texture; }

    private:
        std::pmr::map<char, CharData> charDatas;
        PM3D::CTexture* texture;
    };
}
