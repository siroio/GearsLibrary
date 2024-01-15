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
#include <Debugger.h>

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
    float s_monoMatrixCoefficients[Glib::Internal::Audio::CHANNEL_MONAURAL * Glib::Internal::Audio::CHANNEL_OUTPUT]{ 0.0f };
    float s_steMatrixCoefficients[Glib::Internal::Audio::CHANNEL_STEREO * Glib::Internal::Audio::CHANNEL_OUTPUT]{ 0.0f };

    std::unordered_map<unsigned int, IXAudio2SubmixVoice*> s_subMixVoice;
    std::unordered_map<unsigned int, std::shared_ptr<Glib::AudioClip>> s_audioClips;
}

bool Glib::Internal::Audio::XAudioSystem::Initialize()
{
    // フラグの設定
    UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif
    // XAudio2の初期化
    if (FAILED(XAudio2Create(s_xAudio2.ReleaseAndGetAddressOf(), flags, XAUDIO2_DEFAULT_PROCESSOR)))
        return false;

    if (FAILED(s_xAudio2->CreateMasteringVoice(&s_masterVoice)))
        return false;

    // X3DAudioの初期化
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
    s_X3DAudioDspSettings.at(0).pMatrixCoefficients = s_monoMatrixCoefficients;
    s_X3DAudioDspSettings.at(1).SrcChannelCount = CHANNEL_STEREO;
    s_X3DAudioDspSettings.at(1).pMatrixCoefficients = s_steMatrixCoefficients;

    return true;
}

void Glib::Internal::Audio::XAudioSystem::Finalize()
{
    if (s_masterVoice != nullptr)
    {
        s_masterVoice->DestroyVoice();
        s_masterVoice = nullptr;
    }

    for (auto&& subMixVoice : s_subMixVoice | std::ranges::views::values)
    {
        if (subMixVoice != nullptr)
        {
            subMixVoice->DestroyVoice();
            subMixVoice = nullptr;
        }
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

    // 拡張子を小文字で抜き出す
    const std::filesystem::path file{ path.data() };
    std::string extension = file.filename().extension().generic_string();
    XAUDIO2_BUFFER buffer{};
    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

    // ローダーの作成
    const auto loader = AudioLoaderFactory::Create(extension);
    if (loader == nullptr) return false;

    // AudioClipの読込と登録
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
        destinationVoice = s_subMixVoice.at(groupId);
    }

    voice->SetFrequencyRatio(dspSettings->DopplerFactor);
    voice->SetOutputMatrix(destinationVoice, channelCount, s_channelNum, dspSettings->pMatrixCoefficients);
}

void Glib::Internal::Audio::XAudioSystem::CreateSourceVoice(unsigned id, bool loop, WeakPtr<AudioClip>& clip, IXAudio2SourceVoice** voice)
{
    // voiceを初期化
    if ((*voice) != nullptr)
    {
        (*voice)->Stop();
        (*voice)->DestroyVoice();
        (*voice) = nullptr;
    }

    if (!s_audioClips.contains(id))
    {
        Glib::Debug::Error("Invalid ID: " + std::to_string(id));
        return;
    }

    auto& audioClip = s_audioClips.at(id);
    clip = WeakPtr<AudioClip>{ audioClip };
    clip->Loop(loop);
    s_xAudio2->CreateSourceVoice(voice, &audioClip->Format());
}

void Glib::Internal::Audio::XAudioSystem::CreateSubMixVoice(unsigned groupId)
{
    IXAudio2SubmixVoice* smv{ nullptr };
    XAUDIO2_VOICE_DETAILS details{};

    s_masterVoice->GetVoiceDetails(&details);
    s_xAudio2->CreateSubmixVoice(&smv, details.InputChannels, details.InputSampleRate);
    s_subMixVoice.emplace(groupId, smv);
}

void Glib::Internal::Audio::XAudioSystem::SetOutputSubMixVoice(IXAudio2SourceVoice* sourceVoice, unsigned groupId)
{
    if (sourceVoice == nullptr) return;
    if (!s_subMixVoice.contains(groupId))
    {
        sourceVoice->SetOutputVoices(nullptr);
        return;
    }

    XAUDIO2_SEND_DESCRIPTOR SFXSend{ 0, s_subMixVoice.at(groupId) };
    XAUDIO2_VOICE_SENDS SFXSendList{ 1, &SFXSend };
    sourceVoice->SetOutputVoices(&SFXSendList);
}

void Glib::Internal::Audio::XAudioSystem::SetListenerParameter(const Vector3& position, const Vector3& forward, const Vector3& up)
{
    Vector3 velocity = Vector3::Zero();
    velocity.x = position.x - s_X3DAudioListener.Position.x;
    velocity.y = position.y - s_X3DAudioListener.Position.y;
    velocity.z = position.z - s_X3DAudioListener.Position.z;

    // velocityの計算
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
