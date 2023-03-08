#pragma once

class ResourceManager
{
public:
	ResourceManager(Engine* engine);
	~ResourceManager();

	template<typename T>
	std::shared_ptr<T> CreateResourceFromFile(const wchar_t* file_path)
	{
		return std::dynamic_pointer_cast<T>(CreateResourceFromFileConcrete(file_path));
	}

	Engine* GetEngine() const;

	template<typename T>
	std::shared_ptr<T> GetResource(std::wstring resource_name)
	{
		auto iter = m_resources.find(resource_name);
		if (iter != m_resources.end())
		{
			return std::dynamic_pointer_cast<T>(iter->second);
		}

		return nullptr;
	}

private:
	ResourcePtr CreateResourceFromFileConcrete(const wchar_t* file_path);

	std::unordered_map<std::wstring, ResourcePtr> m_resources;
	Engine* m_engine = nullptr;
};

