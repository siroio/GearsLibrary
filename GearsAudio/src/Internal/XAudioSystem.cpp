#include <Internal/XAudioSystem.h>
#include <Internal/AudioChannels.h>
#include <Internal/AudioLoaderFactory.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <AudioClip.h>
#include <Vector3.h>
#include <GameTimer.h>
#include <ComPtr.h>
#include <filesystem>
#include <unordered_map>
#include <array>
#include <memory>
#include <algorithm>
#include <ranges>

namespace
{
    ComPtr<IXAudio2> s_xAudio2{ nullptr };
    IXAudio2MasteringVoice* s_masterVoice{ nullptr };

    // 3D AUDIO
    X3DAUDIO_HANDLE s_X3DAudioHandle{};
    X3DAUDIO_LISTENER s_X3DAudioListener{};
    std::array<X3DAUDIO_DSP_SETTINGS, 2> s_X3DAudioDspSettings;

    unsigned int s_channelNum{ 0 };

    // CHANNEL SETTINGS
    std::array<float, Glib::Internal::Audio::CHANNEL_MONAURAL* Glib::Internal::Audio::CHANNEL_OUTPUT>
        s_monoMatrixCoefficients;
    std::array<float, Glib::Internal::Audio::CHANNEL_STEREO* Glib::Internal::Audio::CHANNEL_OUTPUT>
        s_steMatrixCoefficients;

    std::unordered_map<unsigned int, std::shared_ptr<IXAudio2SubmixVoice>> s_subMixVoice;
    std::unordered_map<unsigned int, std::shared_ptr<Glib::AudioClip>> s_audioClips;
}

bool Glib::Internal::Audio::XAudioSystem::Initialize()
{
    // ƒtƒ‰ƒO‚ÌÝ’è
    UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif
    // XAudio2‚Ì‰Šú‰»
    if (FAILED(XAudio2Create(s_xAudio2.ReleaseAndGetAddressOf(), flags, XAUDIO2_DEFAULT_PROCESSOR)))
        return false;

    if (FAILED(s_xAudio2->CreateMasteringVoice(&s_masterVoice)))
        return false;

    // X3DAudio‚Ì‰Šú‰»
    DWORD dwChannelMask{};
    if (FAILED(s_masterVoice->GetChannelMask(&dwChannelMask)))
        return false;

    if (FAILED(X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, s_X3DAudioHandle)))
        return false;

    s_X3DAudioListener.OrientFront = { 0.0f, 0.0f, 1.0f };
    s_X3DAudioListener.OrientTop = { 0.0f, 1.0f, 0.0f };

    XAUDIO2_VOICE_DETAILS details{};
    s_masterVoice->GetVoiceDetails(&details);
    s_X3DAudioDspSettings.at(0).DstChannelCount = details.InputChannels;
    s_X3DAudioDspSettings.at(1).DstChannelCount = details.InputChannels;
    s_channelNum = details.InputChannels;
    s_X3DAudioDspSettings.at(0).SrcChannelCount = CHANNEL_MONAURAL;
    s_X3DAudioDspSettings.at(0).pMatrixCoefficients = s_monoMatrixCoefficients.data();
    s_X3DAudioDspSettings.at(1).SrcChannelCount = CHANNEL_STEREO;
    s_X3DAudioDspSettings.at(1).pMatrixCoefficients = s_steMatrixCoefficients.data();

    return true;
}

void Glib::Internal::Audio::XAudioSystem::Finalize()
{
    if (s_masterVoice != nullptr)
    {
        s_masterVoice->DestroyVoice();
        s_masterVoice = nullptr;
    }

    for (auto&& audio : s_subMixVoice | std::ranges::views::values)
    {
        if (audio == nullptr) continue;
        audio->DestroyVoice();
    }
    s_subMixVoice.clear();

    for (auto&& clip : s_audioClips | std::ranges::views::values)
    {
        clip.reset();
    }
    s_audioClips.clear();

    if (s_masterVoice != nullptr)
    {
        s_masterVoice->DestroyVoice();
    }
}

bool Glib::Internal::Audio::XAudioSystem::LoadVoice(unsigned id, std::string_view path)
{
    if (s_audioClips.contains(id)) return true;

    // Šg’£Žq‚ð¬•¶Žš‚Å”²‚«o‚·
    const std::filesystem::path file{ path.data() };
    std::string extension = file.filename().extension().generic_string();
    XAUDIO2_BUFFER buffer{};
    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

    // ƒ[ƒ_[‚Ìì¬
    const auto loader = AudioLoaderFactory::Create(extension);
    if (loader == nullptr) return false;

    // AudioClip‚Ì“Çž‚Æ“o˜^
    const auto audioClip = loader->Load(path);
    s_audioClips.emplace(id, audioClip);
    return true;
}

void Glib::Internal::Audio::XAudioSystem::Audio3DCalculate(const X3DAUDIO_EMITTER* emitter, IXAudio2SourceVoice* voice, unsigned groupId)
{
    if (voice == nullptr) return;
    const UINT32 channelCount = emitter->ChannelCount;
    X3DAUDIO_DSP_SETTINGS* dspSettings;
    if (channelCount == 1) dspSettings = &s_X3DAudioDspSettings.at(0);
    else if (channelCount == 2) dspSettings = &s_X3DAudioDspSettings.at(1);
    else return;

    constexpr DWORD flags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT;
    X3DAudioCalculate(s_X3DAudioHandle, &s_X3DAudioListener, emitter, flags, dspSettings);

    IXAudio2Voice* destinationVoice = s_masterVoice;
    if (s_subMixVoice.contains(groupId))
    {
        destinationVoice = s_subMixVoice.at(groupId).get();
    }

    voice->SetFrequencyRatio(dspSettings->DopplerFactor);
    voice->SetOutputMatrix(destinationVoice, channelCount, s_channelNum, dspSettings->pMatrixCoefficients);
}

void Glib::Internal::Audio::XAudioSystem::CreateSourceVoice(unsigned id, bool loop, WeakPtr<AudioClip>& clip, IXAudio2SourceVoice** voice)
{
    // voice‚ð‰Šú‰»
    if (auto* pVoice = *voice; pVoice != nullptr)
    {
        pVoice->Stop();
        pVoice->DestroyVoice();
        pVoice = nullptr;
    }

    // TODO: ERROR Handling => invalid id
    if (!s_audioClips.contains(id)) return;
    clip = s_audioClips.at(id);
    clip->Loop(loop);
    s_xAudio2->CreateSourceVoice(voice, &clip->Format());
}

void Glib::Internal::Audio::XAudioSystem::CreateSubMixVoice(unsigned groupId)
{
    IXAudio2SubmixVoice* smv{ nullptr };
    XAUDIO2_VOICE_DETAILS details{};
    s_masterVoice->GetVoiceDetails(&details);
    s_xAudio2->CreateSubmixVoice(&smv, details.InputChannels, details.InputSampleRate);
    s_subMixVoice[groupId] = std::shared_ptr<IXAudio2SubmixVoice>{ smv };
}

void Glib::Internal::Audio::XAudioSystem::SetOutputSubMixVoice(IXAudio2SourceVoice* sourceVoice, unsigned groupId)
{
    if (sourceVoice == nullptr) return;
    if (s_subMixVoice.contains(groupId))
    {
        sourceVoice->SetOutputVoices(nullptr);
        return;
    }

    const auto smv = s_subMixVoice.at(groupId);
    XAUDIO2_SEND_DESCRIPTOR sendDescriptor{ 0, smv.get() };
    const XAUDIO2_VOICE_SENDS voiceSends{ 1, &sendDescriptor };
    sourceVoice->SetOutputVoices(&voiceSends);
}

void Glib::Internal::Audio::XAudioSystem::SetListenerParameter(const Vector3& position, const Vector3& forward, const Vector3& up)
{
    Vector3 velocity{
        position.x - s_X3DAudioListener.Position.x,
        position.y - s_X3DAudioListener.Position.y,
        position.z - s_X3DAudioListener.Position.z
    };

    // velocity‚ÌŒvŽZ
    const float deltaTime = GameTimer::DeltaTime();
    velocity = deltaTime == 0.0f ?
        Vector3::Zero() :
        velocity.Normalized() / deltaTime;

    s_X3DAudioListener.Velocity = { velocity.x, velocity.y, velocity.z };
    s_X3DAudioListener.Position = { position.x, position.y, position.z };
    s_X3DAudioListener.OrientFront = { forward.x, forward.y, forward.z };
    s_X3DAudioListener.OrientTop = { up.x, up.y, up.z };
}

void Glib::Internal::Audio::XAudioSystem::SetGroupVolume(unsigned int groupId, float volume)
{
    if (!s_subMixVoice.contains(groupId)) return;
    s_subMixVoice.at(groupId)->SetVolume(volume);
}
