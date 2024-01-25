#include <Components/EffectSystem.h>
#include <Internal/EffekseerManager.h>
#include <GameObject.h>
#include <Matrix4x4.h>

namespace
{
    auto s_efkManager = Glib::Internal::Effect::EffekseerManager::Instance();
}

void Glib::EffectSystem::Start()
{
    transform_ = GameObject()->Transform();
    effectState_.Set(EffectState::Looping, true);
    if (effectState_ == EffectState::PlayOnStart) Play();
}

void Glib::EffectSystem::LateUpdate()
{
    if (!IsPlaying()) return;

    if (!s_efkManager->Exists(effectHandle_))
    {
        effectHandle_ = -1;
        if (effectState_ == EffectState::Looping)
        {
            Play();
        }
        else if (effectState_ == EffectState::OnStop)
        {
            GameObject()->Destroy();
            return;
        }
    }

    s_efkManager->SetMatrix(
        effectHandle_,
        Matrix4x4::TRS(transform_->Position(), transform_->Rotation(), transform_->Scale())
    );
}

void Glib::EffectSystem::Play()
{
    if (!IsPlaying()) return;

    if (effectState_ == EffectState::Pause)
    {
        s_efkManager->UnPause(effectHandle_);
        effectState_.Set(EffectState::Looping, false);
    }
    else
    {
        Stop();
        effectHandle_ = s_efkManager->Play(effectID_, GameObject()->Transform()->Position());
    }
}

void Glib::EffectSystem::Pause()
{
    effectState_.Set(EffectState::Pause, true);
    s_efkManager->Pause(effectHandle_);
}

void Glib::EffectSystem::Stop()
{
    effectState_.Set(EffectState::Pause, false);
    s_efkManager->Stop(effectHandle_);
    effectHandle_ = -1;
}

bool Glib::EffectSystem::IsPlaying() const
{
    return effectHandle_ >= 0;
}

unsigned int Glib::EffectSystem::EffectID() const
{
    return effectID_;
}

void Glib::EffectSystem::EffectID(unsigned int id)
{
    effectID_ = id;
}
