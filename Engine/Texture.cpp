#include "pch.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "App.h"
#include "Graphics.h"
#include "Renderer.h"

Texture::Texture(const wchar_t* file_path, ResourceManager* resource_manager)
	: Resource(file_path, resource_manager)
{
	m_texture = resource_manager->GetApp()->GetGraphics()->GetRenderer()->CreateTexture(file_path);
}

Texture::~Texture()
{
}

const Texture2DPtr& Texture::GetTexture() const
{
	return m_texture;
}
