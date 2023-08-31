#include <Components/AudioSource.h>
#include <Internal/XAudioSystem.h>
#include <Mathf.h>
#include <GameObject.h>
#include <GameTimer.h>

namespace
{
    auto s_xAudio2 = Glib::Internal::Audio::XAudioSystem::Instance();
}

Glib::AudioSource::AudioSource()
{
    // エミッターの初期化
    emitter_.OrientFront = { 0.0f, 0.0f, 1.0f };
    emitter_.OrientTop = { 0.0f, 1.0f, 0.0f };
    emitter_.ChannelRadius = 1.0f;
    emitter_.pVolumeCurve = const_cast<X3DAUDIO_DISTANCE_CURVE*>(&X3DAudioDefault_LinearCurve);
    emitter_.CurveDistanceScaler = 14.0f;
    emitter_.DopplerScaler = 1.0f;
    emitter_.InnerRadius = 2.0f;
    emitter_.InnerRadiusAngle = Mathf::PI / 4.0f;
}

Glib::AudioSource::~AudioSource()
{
    if (sourceVoice_ != nullptr)
    {
        sourceVoice_->Stop();
        sourceVoice_->DestroyVoice();
        sourceVoice_ = nullptr;
    }
}

void Glib::AudioSource::Start()
{
    if (audioStatus_ == AudioStatus::PlayOnStart) Play();
}

void Glib::AudioSource::LateUpdate()
{
    if (audioStatus_.Has(AudioStatus::Is3DAudio))
    {
        UpdateVoice();
    }
}

void Glib::AudioSource::Play()
{
    if (sourceVoice_ == nullptr) return;
    if (audioStatus_ == AudioStatus::Pause)
    {
        sourceVoice_->Stop();
        sourceVoice_->FlushSourceBuffers();
        sourceVoice_->SubmitSourceBuffer(&audioClip_->Buffer());
    }

    audioStatus_.Clear(AudioStatus::Pause);
    sourceVoice_->Start();
}

void Glib::AudioSource::Pause()
{
    if (sourceVoice_ == nullptr) return;
    audioStatus_.Set(AudioStatus::Pause);
    sourceVoice_->Stop();
}

void Glib::AudioSource::Stop()
{
    if (sourceVoice_ == nullptr) return;
    audioStatus_.Clear(AudioStatus::Pause);
    sourceVoice_->Stop();
    sourceVoice_->FlushSourceBuffers();
    sourceVoice_->SubmitSourceBuffer(&audioClip_->Buffer());
}

bool Glib::AudioSource::PlayOnStart() const
{
    return audioStatus_.Has(AudioStatus::PlayOnStart);
}

void Glib::AudioSource::PlayOnStart(bool enable)
{
    audioStatus_.Set(AudioStatus::PlayOnStart, enable);
}

bool Glib::AudioSource::Is3DSound() const
{
    return audioStatus_.Has(AudioStatus::Is3DAudio);
}

void Glib::AudioSource::Is3DSound(bool enable)
{
    audioStatus_.Set(AudioStatus::Is3DAudio, enable);
}

bool Glib::AudioSource::Loop() const
{
    return audioClip_->Loop();
}

void Glib::AudioSource::Loop(bool enable)
{
    audioClip_->Loop(enable);
    if (!enable) sourceVoice_->ExitLoop();
}

float Glib::AudioSource::Volume() const
{
    if (sourceVoice_ == nullptr) return 0.0f;
    float volume;
    sourceVoice_->GetVolume(&volume);
    return volume;
}

void Glib::AudioSource::Volume(float volume)
{
    if (sourceVoice_ == nullptr) return;
    sourceVoice_->SetVolume(Mathf::Clamp01(volume));
}

unsigned int Glib::AudioSource::AudioID() const
{
    return audioId_;
}

void Glib::AudioSource::AudioID(unsigned int id)
{
    if (audioId_ == id) return;
    audioId_ = id;
    s_xAudio2->CreateSourceVoice(id, Loop(), audioClip_, &sourceVoice_);
    if (sourceVoice_ == nullptr) return;
    sourceVoice_->SubmitSourceBuffer(&audioClip_->Buffer());

    // グループに割り当てられている
    if (groupId_ >= 0) SetGroup(groupId_);

    // 3Dオーディオの設定
    if (audioStatus_ != AudioStatus::Is3DAudio) return;
    const unsigned int channel = audioClip_->Channels();
    emitter_.ChannelCount = channel;
    emitterAzimuths_.reset(new float[channel]);
    emitter_.pChannelAzimuths = emitterAzimuths_.get();
    UpdateVoice();
}

void Glib::AudioSource::SetGroup(int group)
{
    if (sourceVoice_ == nullptr) return;
    s_xAudio2->SetOutputSubMixVoice(sourceVoice_, group);
}

void Glib::AudioSource::UpdateVoice()
{
    if (sourceVoice_ == nullptr) return;
    const auto& transform = GameObject()->Transform();
    const Vector3 position = transform->Position();
    const Vector3 forward = transform->Forward();
    const Vector3 up = transform->Up();

    Vector3 velocity{
        position.x - emitter_.Position.x,
        position.y - emitter_.Position.y,
        position.z - emitter_.Position.z
    };

    const float deltaTime = GameTimer::DeltaTime();
    velocity = deltaTime == 0.0f ? Vector3::Zero() : velocity.Normalized() / deltaTime;

    emitter_.Velocity = { velocity.x, velocity.y, velocity.z };
    emitter_.Position = { position.x, position.y, position.z };
    emitter_.OrientFront = { forward.x, forward.y, forward.z };
    emitter_.OrientTop = { up.x, up.y, up.z };

    s_xAudio2->Audio3DCalculate(&emitter_, sourceVoice_, groupId_);
}
