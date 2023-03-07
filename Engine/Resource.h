#pragma once

class Resource
{
public:
	Resource(const wchar_t* file_path, ResourceManager* resource_manager);
	virtual ~Resource();

protected:
	const wchar_t* m_file_path = L"";
	ResourceManager* m_resource_manager = nullptr;
};

