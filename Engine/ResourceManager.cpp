#include "pch.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

ResourceManager::ResourceManager(App* app)
    : m_app(app)
{
}

ResourceManager::~ResourceManager()
{
}

App* ResourceManager::GetApp() const
{
    return m_app;
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
    else if (!extension.compare(L".jpg") || !extension.compare(L".png") || !extension.compare(L".bmp"))
        resource_ptr = std::make_shared<Texture>(resource_path.c_str(), this);
    else if (!extension.compare(L".hlsl") || !extension.compare(L".fx"))
        resource_ptr = std::make_shared<Material>(resource_path.c_str(), this);

    if (resource_ptr)
    {
        m_resources.emplace(file_path, resource_ptr);
        return resource_ptr;
    }

    return nullptr;
}
