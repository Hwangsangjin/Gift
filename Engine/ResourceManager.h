#pragma once

class Resource;

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourcePtr CreateResourceFromFile(const wchar_t* file_path);

protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* file_path) = 0;

private:
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;
};

