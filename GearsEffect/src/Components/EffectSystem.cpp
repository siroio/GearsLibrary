#include <Components/EffectSystem.h>
#include <Internal/EffekseerManager.h>
#include <GameObject.h>
#include <Matrix4x4.h>
#include <GLGUI.h>

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

    UpdateSpeed();
    UpdateTarget();
    UpdateColor();
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
    UpdateSpeed();
}

Vector3 Glib::EffectSystem::TargetPosition() const
{
    return effectTarget_;
}

void Glib::EffectSystem::TargetPosition(const Vector3& targetPosition)
{
    effectTarget_ = targetPosition;
    UpdateTarget();
}

Color Glib::EffectSystem::EffectColor() const
{
    return effectColor_;
}

void Glib::EffectSystem::EffectColor(const Color& color)
{
    effectColor_ = color;
    UpdateColor();
}

void Glib::EffectSystem::UpdateSpeed()
{
    if (!IsPlaying()) return;
    s_efkManager->SetSpeed(effectHandle_, effectSpeed_);
}

void Glib::EffectSystem::UpdateTarget()
{
    if (!IsPlaying()) return;
    s_efkManager->SetTargetPosition(effectHandle_, effectTarget_);
}

void Glib::EffectSystem::UpdateColor()
{
    if (!IsPlaying()) return;
    s_efkManager->SetColor(effectHandle_, effectColor_);
}

void Glib::EffectSystem::OnGUI()
{
    int id = static_cast<int>(effectID_);
    if (GLGUI::InputInt("Effect ID", &id))
    {
        effectID_ = static_cast<unsigned short>(id);
    }

    if (GLGUI::Button("Play")) Play();
    if (GLGUI::Button("Pause", true)) Pause();
    if (GLGUI::Button("Stop", true)) Stop();

    bool isLoop = effectState_ == EffectState::Looping;
    if (GLGUI::CheckBox("Loop", &isLoop))
    {
        effectState_.Set(EffectState::Looping, isLoop);
    }

    bool destoryOnFinish = effectState_ == EffectState::DestoryOnFinish;
    if (GLGUI::CheckBox("Destroy On End", &destoryOnFinish))
    {
        effectState_.Set(EffectState::DestoryOnFinish, destoryOnFinish);
    }

    if (GLGUI::DragFloat("Speed", &effectSpeed_, 0.001f, 0.0f))
    {
        UpdateSpeed();
    }

    if (GLGUI::DragVector3("Target", &effectTarget_))
    {
        UpdateTarget();
    }

    if (GLGUI::ColorInput4("Color", &effectColor_))
    {
        UpdateColor();
    }
}
