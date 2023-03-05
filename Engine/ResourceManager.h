#pragma once

class ResourceManager
{
public:
	ResourceManager(App* app);
	~ResourceManager();

	template<typename T>
	std::shared_ptr<T> CreateResourceFromFile(const wchar_t* file_path)
	{
		return std::dynamic_pointer_cast<T>(CreateResourceFromFileConcrete(file_path));
	}

	App* GetApp();

private:
	ResourcePtr CreateResourceFromFileConcrete(const wchar_t* file_path);

	std::unordered_map<std::wstring, ResourcePtr> m_resources;
	App* m_app = nullptr;
};

