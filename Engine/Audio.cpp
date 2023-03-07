#include "pch.h"
#include "Audio.h"

Audio::Audio(const wchar_t* file_path, ResourceManager* resource_manager)
    : Resource(file_path, resource_manager)
{
    m_file_path = file_path;
}

Audio::~Audio()
{
}

std::wstring& Audio::GetFilePath()
{
    return m_file_path;
}
