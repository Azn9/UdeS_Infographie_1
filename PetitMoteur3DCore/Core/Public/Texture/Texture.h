#pragma once

#include <string>
#include "../../Public/Core/dispositifD3D11.h"
#include <vector>

namespace PM3D
{

class CTexture
{
public:
	CTexture(const std::wstring& filename, CDispositifD3D11* pDispositif);
	CTexture(const std::wstring& name, const std::vector<std::wstring>& filenames, CDispositifD3D11* pDispositif);
	~CTexture();

	const std::wstring& GetName() const { return m_Name; }
	ID3D11ShaderResourceView* GetD3DTexture() { return m_TextureSRV; }
	

protected:
	std::wstring m_Name;
	ID3D11ShaderResourceView* m_TextureSRV;
	int m_TextureCount;
};


}
