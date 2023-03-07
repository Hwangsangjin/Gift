#pragma once

#include "Resource.h"

class Texture : public Resource
{
public:
	Texture(const wchar_t* file_path, ResourceManager* resource_manager);
	virtual ~Texture();

	const Texture2DPtr& GetTexture() const;

private:
	Texture2DPtr m_texture = nullptr;
};

