#include "pch.h"
#include "ResourceManager.h"

#if __cplusplus <= 201402L 
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#endif

#if __cplusplus >= 201703L
#include <filesystem>
#endif

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourcePtr ResourceManager::CreateResourceFromFile(const wchar_t* file_path)
{
    std::wstring full_path = std::experimental::filesystem::absolute(file_path);

    auto iter = m_map_resources.find(full_path);
    if (iter != m_map_resources.end())
        return iter->second;

    Resource* raw_res = CreateResourceFromFileConcrete(full_path.c_str());
    if (raw_res)
    {
        ResourcePtr res(raw_res);
        m_map_resources[full_path] = res;
        return res;
    }

    return nullptr;
}
