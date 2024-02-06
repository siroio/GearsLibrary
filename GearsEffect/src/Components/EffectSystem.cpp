#include <Components/EffectSystem.h>
#include <Internal/EffekseerManager.h>
#include <GameObject.h>
#include <Matrix4x4.h>

namespace
{
    auto s_efkManager = Glib::Internal::Effect::EffekseerManager::Instance();
}

Glib::EffectSystem::~EffectSystem()
{
    if (IsPlaying()) Stop();
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

    // エフェクトインスタンスの存在チェック
    if (!s_efkManager->Exists(effectHandle_))
    {
        effectHandle_ = EFFEKSEER_INVALID_HANDLE;
        if (effectState_ == EffectState::Looping)
        {
            Play();
        }
        else if (effectState_ == EffectState::DestoryOnFinish)
        {
            GameObject()->Destroy();
            return;
        }
    }

    // (位置 回転 拡大縮小)を同期
    s_efkManager->SetMatrix(
        effectHandle_,
        Matrix4x4::TRS(transform_->Position(), transform_->Rotation(), transform_->Scale())
    );
}

void Glib::EffectSystem::Play()
{
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

    s_efkManager->SetSpeed(effectHandle_, effectSpeed_);
    s_efkManager->SetTargetPosition(effectHandle_, effectTarget_);
    s_efkManager->SetColor(effectHandle_, effectColor_);
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
    effectHandle_ = EFFEKSEER_INVALID_HANDLE;
}

bool Glib::EffectSystem::IsPlaying() const
{
    return effectHandle_ > EFFEKSEER_INVALID_HANDLE;
}

bool Glib::EffectSystem::Loop() const
{
    return effectState_.Has(EffectState::Looping);
}

void Glib::EffectSystem::Loop(bool enable)
{
    effectState_.Set(EffectState::Looping, enable);
}

bool Glib::EffectSystem::DestoryOnFinish() const
{
    return effectState_.Has(EffectState::DestoryOnFinish);
}

void Glib::EffectSystem::DestoryOnFinish(bool enable)
{
    effectState_.Set(EffectState::DestoryOnFinish, enable);
}

bool Glib::EffectSystem::PlayOnStart() const
{
    return effectState_.Has(EffectState::PlayOnStart);
}

void Glib::EffectSystem::PlayOnStart(bool enable)
{
    effectState_.Set(EffectState::PlayOnStart, enable);
}

unsigned int Glib::EffectSystem::EffectID() const
{
    return effectID_;
}

void Glib::EffectSystem::EffectID(unsigned int id)
{
    effectID_ = id;
}

float Glib::EffectSystem::Speed() const
{
    return effectSpeed_;
}

void Glib::EffectSystem::Speed(float speed)
{
    effectSpeed_ = speed;
}

Vector3 Glib::EffectSystem::TargetPosition() const
{
    return effectTarget_;
}

void Glib::EffectSystem::TargetPosition(const Vector3& targetPosition)
{
    effectTarget_ = targetPosition;
}

Color Glib::EffectSystem::EffectColor() const
{
    return effectColor_;
}

void Glib::EffectSystem::EffectColor(const Color& color)
{
    effectColor_ = color;
}
