#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
    : ResourceManager()
{
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::CreateTextureFromFile(const wchar_t* file_path)
{
    return std::static_pointer_cast<Texture>(CreateResourceFromFile(file_path));
}

Resource* TextureManager::CreateResourceFromFileConcrete(const wchar_t* file_path)
{
    Texture* texture = nullptr;
    try
    {
        texture = new Texture(file_path);
    }
    catch (...)
    {
        assert(texture);
        throw std::exception("Texture not created successfully");
    }

    return texture;
}
