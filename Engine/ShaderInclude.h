#pragma once

class ShaderInclude : public ID3DInclude
{
public:
	ShaderInclude(const std::filesystem::path& file_path);
	~ShaderInclude();

	virtual HRESULT Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes);
	virtual HRESULT Close(THIS_ LPCVOID pData);

private:
	std::filesystem::path m_file_path;
};

