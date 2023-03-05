#include "pch.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "App.h"
#include "Graphics.h"
#include "Renderer.h"

Texture::Texture(const wchar_t* full_path, ResourceManager* resource_manager)
	: Resource(full_path, resource_manager)
{
	m_texture = resource_manager->GetApp()->GetGraphics()->GetRenderer()->CreateTexture(full_path);
}

const Texture2DPtr& Texture::GetTexture() const
{
	return m_texture;
}
