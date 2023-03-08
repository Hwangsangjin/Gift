#include "pch.h"
#include "Resource.h"
#include "ResourceManager.h"

Resource::Resource(const wchar_t* file_path, ResourceManager* resource_manager)
	: m_file_path(file_path)
	, m_resource_manager(resource_manager)
{
}

Resource::~Resource()
{
}

const std::wstring& Resource::GetFilePath() const
{
	return m_file_path;
}
