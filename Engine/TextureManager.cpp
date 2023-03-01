#include "pch.h"
#include "TextureManager.h"

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

TexturePtr TextureManager::CreateTexture(const Rect& rect, Texture::Type type)
{
    Texture* texture = new Texture(rect, type);
    assert(texture);
    TexturePtr texture_ptr(texture);
    assert(texture_ptr);
    return texture_ptr;
}

Resource* TextureManager::CreateResourceFromFileConcrete(const wchar_t* file_path)
{
    Texture* texture = new Texture(file_path);
    assert(texture);
    return texture;
}
