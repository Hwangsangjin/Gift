#include "pch.h"
#include "ShaderInclude.h"

ShaderInclude::ShaderInclude(const std::filesystem::path& file_path)
	: m_file_path(file_path)
{
}

ShaderInclude::~ShaderInclude()
{
}

HRESULT ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
	auto file_path = m_file_path;
	file_path = file_path.parent_path();
	file_path += '/';
	file_path += pFileName;

	std::ifstream shader_stream(file_path);
	if (shader_stream.is_open())
	{
		std::stringstream sstr;
		sstr << shader_stream.rdbuf();
		auto str = sstr.str();

		char* shader_code = new char[str.size() + 1];
		memcpy(shader_code, str.c_str(), str.size() + 1);

		shader_stream.close();

		*ppData = shader_code;
		*pBytes = static_cast<UINT>(str.size());

		return S_OK;
	}

	return S_FALSE;
}

HRESULT ShaderInclude::Close(LPCVOID pData)
{
	delete pData;
	return S_OK;
}
