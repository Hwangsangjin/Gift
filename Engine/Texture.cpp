#include "pch.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "RenderSystem.h"

Texture::Texture(const wchar_t* file_path, ResourceManager* resource_manager)
	: Resource(file_path, resource_manager)
{
	m_texture = resource_manager->GetEngine()->GetRenderSystem()->CreateTexture(file_path);
}

Texture::~Texture()
{
}

const Texture2DPtr& Texture::GetTexture() const
{
	return m_texture;
}
