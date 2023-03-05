#include "pch.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "App.h"
#include "Engine.h"
#include "Graphics.h"

Texture::Texture(const wchar_t* full_path, ResourceManager* resource_manager)
	: Resource(full_path, resource_manager)
{
	m_texture = resource_manager->GetApp()->GetEngine()->GetGraphics()->CreateTexture(full_path);
}

const Texture2DPtr& Texture::GetTexture() const
{
	return m_texture;
}
