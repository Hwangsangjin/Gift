#include "pch.h"
#include "Audio.h"
#include "ResourceManager.h"
#include "SoundSystem.h"
#include "Engine.h"

Audio::Audio(const wchar_t* file_path, ResourceManager* resource_manager)
    : Resource(file_path, resource_manager)
{
	m_resource_manager->GetEngine()->GetSoundSystem()->CreateSound(this);
}

Audio::~Audio()
{
}
