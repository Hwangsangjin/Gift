#pragma once

class Resource
{
public:
	Resource(const wchar_t* file_path, ResourceManager* manager);
	virtual ~Resource();

protected:
	std::wstring m_file_path = L"";
	ResourceManager* m_resource_manager = nullptr;
};

