#include <Components/AudioListener.h>
#include <Internal/XAudioSystem.h>
#include <Components/Transform.h>
#include <GameObject.h>

namespace
{
    auto s_xAudio2 = Glib::Internal::Audio::XAudioSystem::Instance();
}

void Glib::AudioListener::Start()
{
    transform_ = GameObject()->Transform();
}

void Glib::AudioListener::LateUpdate()
{
    // パラメーターの更新
    s_xAudio2->SetListenerParameter(
        transform_->Position(),
        transform_->Forward(),
        transform_->Up()
    );
}
