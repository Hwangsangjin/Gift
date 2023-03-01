#pragma once

#include "ResourceManager.h"
#include "Texture.h"

class Rect;

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	virtual ~TextureManager() override;

	TexturePtr CreateTextureFromFile(const wchar_t* file_path);
	TexturePtr CreateTexture(const Rect& rect, Texture::Type type);

protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* file_path) override;
};

