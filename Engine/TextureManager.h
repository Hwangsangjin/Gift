#pragma once

#include "ResourceManager.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	virtual ~TextureManager() override;

	TexturePtr CreateTextureFromFile(const wchar_t* file_path);

protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* file_path) override;
};

