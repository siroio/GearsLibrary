#include <AudioClip.h>
#include <Internal/XAudioSystem.h>

namespace
{
    auto s_xAudio2 = Glib::Internal::Audio::XAudioSystem::Instance();
}

Glib::AudioClip::AudioClip(std::string_view name, const WAVEFORMATEX& format, const std::vector<unsigned char>& data) :
    clipName_{ name },
    format_{ format },
    audioData_{ data }
{
    // バッファの作成
    buffer_.pAudioData = reinterpret_cast<BYTE*>(audioData_.data());
    buffer_.Flags = XAUDIO2_END_OF_STREAM;
    buffer_.AudioBytes = static_cast<UINT32>(audioData_.size());
    buffer_.LoopCount = 0;
}

std::string Glib::AudioClip::Name() const
{
    return clipName_;
}

const WAVEFORMATEX& Glib::AudioClip::Format() const
{
    return format_;
}

const std::vector<unsigned char>& Glib::AudioClip::RawData() const
{
    return audioData_;
}

const XAUDIO2_BUFFER& Glib::AudioClip::Buffer() const
{
    return buffer_;
}

unsigned int Glib::AudioClip::Channels() const
{
    return format_.nChannels;
}

bool Glib::AudioClip::Loop() const
{
    return buffer_.LoopCount == XAUDIO2_LOOP_INFINITE;
}

void Glib::AudioClip::Loop(bool enable)
{
    buffer_.LoopCount = enable ? XAUDIO2_LOOP_INFINITE : 0;
}

bool Glib::AudioClip::Load(unsigned int id, std::string_view filePath)
{
    return s_xAudio2->LoadVoice(id, filePath);
}
