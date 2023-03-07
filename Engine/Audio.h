#pragma once

#include "Resource.h"

class Audio : public Resource
{
public:
	Audio(const wchar_t* file_path, ResourceManager* manager);
	virtual ~Audio();

	std::wstring& GetFilePath();

private:
	std::wstring m_file_path;
};
