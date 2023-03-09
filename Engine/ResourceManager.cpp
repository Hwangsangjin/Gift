#include "pch.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Audio.h"

ResourceManager::ResourceManager(Engine* engine)
    : m_engine(engine)
{
}

ResourceManager::~ResourceManager()
{
}

Engine* ResourceManager::GetEngine() const
{
    return m_engine;
}

ResourcePtr ResourceManager::CreateResourceFromFileConcrete(const wchar_t* file_path)
{
    std::filesystem::path resource_path = file_path;
    auto extension = resource_path.extension();

    auto iter = m_resources.find(file_path);
    if (iter != m_resources.end())
    {
        auto material = std::dynamic_pointer_cast<Material>(iter->second);
        if (material)
            return std::make_shared<Material>(material, this);

        return iter->second;
    }

    if (!std::filesystem::exists(resource_path))
        return ResourcePtr();

    ResourcePtr resource_ptr = nullptr;

    if (!extension.compare(L".obj"))
        resource_ptr = std::make_shared<Mesh>(resource_path.c_str(), this);
    else if (!extension.compare(L".jpg") || !extension.compare(L".png") || !extension.compare(L".bmp") || !extension.compare(L".dds"))
        resource_ptr = std::make_shared<Texture>(resource_path.c_str(), this);
    else if (!extension.compare(L".hlsl") || !extension.compare(L".fx"))
        resource_ptr = std::make_shared<Material>(resource_path.c_str(), this);
    else if (!extension.compare(L".mp3") || !extension.compare(L".ogg"))
        resource_ptr = std::make_shared<Audio>(resource_path.c_str(), this);

    if (resource_ptr)
    {
        std::wstring file_name = resource_path.filename().replace_extension().wstring();
        m_resources.emplace(std::make_pair(file_name, resource_ptr));
        return resource_ptr;
    }

    return nullptr;
}
