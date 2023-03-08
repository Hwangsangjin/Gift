#pragma once

#include "Resource.h"

class Audio : public Resource
{
public:
	Audio(const wchar_t* file_path, ResourceManager* manager);
	virtual ~Audio();

private:
	FMOD::Sound* m_sound = nullptr;

	friend SoundSystem;
};
