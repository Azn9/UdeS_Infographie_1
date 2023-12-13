#pragma once
#include <map>
#include <string>

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
	FontLoader(const std::string& fileName);

	CharData GetCharData(const char c) const { return charDatas.at(c); }

private:
	std::pmr::map<char, CharData> charDatas;
};
}
