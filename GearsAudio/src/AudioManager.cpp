#include <AudioManager.h>
#include <Internal/XAudioSystem.h>

namespace
{
    auto s_xAudio = Glib::Internal::Audio::XAudioSystem::Instance();
}

void Glib::AudioManager::AddSoundGroup(unsigned int groupId)
{
    s_xAudio->CreateSubMixVoice(groupId);
}

void Glib::AudioManager::LoadVoice(unsigned int groupId, std::string_view path)
{
    s_xAudio->LoadVoice(groupId, path);
}

void Glib::AudioManager::SetSoundGroupVolume(unsigned groupId, float volume)
{
    s_xAudio->SetGroupVolume(groupId, volume);
}
