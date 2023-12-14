#include "FontLoader.h"

#include <fstream>

#include "Core/Public/Core/MoteurWindows.h"
#include "Core/Public/Texture/GestionnaireDeTextures.h"

PM3D_API::FontLoader::FontLoader(const std::string& fntFileName, const std::wstring& ddsFile)
{
    std::ifstream fntStream(fntFileName);

    if (!fntStream.is_open())
        throw std::runtime_error("Could not open " + fntFileName);

    texture = PM3D::CMoteurWindows::GetInstance().GetTextureManager().GetNewTexture(ddsFile, GameHost::GetInstance()->GetDispositif());

    for (std::string line; getline(fntStream, line);)
    {
        if (line.find("char ") == 0)
        {
            const auto data = line.substr(5);
            const auto id = std::stoi(data.substr(data.find("id=") + 3, data.find(' ') - 3));
            const auto x = std::stoi(data.substr(data.find("x=") + 2, data.find(' ', data.find("x=")) - 2));
            const auto y = std::stoi(data.substr(data.find("y=") + 2, data.find(' ', data.find("y=")) - 2));
            const auto width = std::stoi(data.substr(data.find("width=") + 6, data.find(' ', data.find("width=")) - 6));
            const auto height = std::stoi(data.substr(data.find("height=") + 7,
                                                      data.find(' ', data.find("height=")) - 7));
            const auto xoffset = std::stoi(data.substr(data.find("xoffset=") + 8,
                                                       data.find(' ', data.find("xoffset=")) - 8));
            const auto yoffset = std::stoi(data.substr(data.find("yoffset=") + 8,
                                                       data.find(' ', data.find("yoffset=")) - 8));
            const auto xadvance = std::stoi(
                data.substr(data.find("xadvance=") + 9, data.find(' ', data.find("xadvance=")) - 9));
            const auto page = std::stoi(data.substr(data.find("page=") + 5, data.find(' ', data.find("page=")) - 5));
            const auto chnl = std::stoi(data.substr(data.find("chnl=") + 5, data.find(' ', data.find("chnl=")) - 5));
            const auto letter = data.substr(data.find("letter=\"") + 8,
                                            data.find('\"', data.find("letter=\"") + 8) - 8);

            char c;
            if (letter != "\"\"")
                if (letter == "space" || letter == "s")
                    c = ' ';
                else
                    c = letter[0];
            else c = '"';

            if (width == 0 && height == 0)
            {
                c = ' ';
            }

            const auto charData = CharData{c, x, y, width, height, xoffset, yoffset, xadvance};
            this->charDatas.insert({c, charData});
        }
    }
}
