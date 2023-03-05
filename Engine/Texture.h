#pragma once

#include "Resource.h"

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path, ResourceManager* resource_manager);

	const Texture2DPtr& GetTexture() const;

private:
	Texture2DPtr m_texture;
};

