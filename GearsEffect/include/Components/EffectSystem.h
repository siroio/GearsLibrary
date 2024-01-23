#pragma once
#include <Component.h>
#include <Components/Transform.h>
#include <BitFlag.h>
#include <Vector3.h>
#include <Color.h>

namespace Glib
{
    class EffectSystem : public Component
    {
        enum class EffectState
        {
            Pause,
            Looping,
            OnStop,
            PlayOnStart
        };

    public:
        void Start();
        void LateUpdate();

        void Play();
        void Pause();
        void Stop();

        bool IsPlaying() const;
        unsigned int EffectID() const;
        void EffectID(unsigned int id);

    private:
        unsigned int effectID_{ UINT_MAX };
        int effectHandle_{ -1 };
        float effectSpeed_{ 1.0f };
        Vector3 effectTarget_{ 0.0f, 0.0, 0.0f };
        BitFlag<EffectState> effectState_{};
        WeakPtr<Transform> transform_{ nullptr };
    };
}
