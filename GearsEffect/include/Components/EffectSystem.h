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
            Pause = 1 << 0,
            Looping = 1 << 1,
            DestoryOnFinish = 1 << 2,
            PlayOnStart = 1 << 3
        };

    public:
        ~EffectSystem();
        void Start();
        void LateUpdate();

        void Play();
        void Pause();
        void Stop();

        bool IsPlaying() const;

        bool Loop() const;
        void Loop(bool enable);

        bool DestoryOnFinish() const;
        void DestoryOnFinish(bool enable);

        bool PlayOnStart() const;
        void PlayOnStart(bool enable);

        unsigned int EffectID() const;
        void EffectID(unsigned int id);

        float Speed() const;
        void Speed(float speed);

        Vector3 TargetPosition() const;
        void TargetPosition(const Vector3& targetPosition);

        Color EffectColor() const;
        void EffectColor(const Color& color);

    private:
        void UpdateSpeed();
        void UpdateTarget();
        void UpdateColor();

        void OnGUI() override;

    private:
        unsigned int effectID_{ UINT_MAX };
        int effectHandle_{ -1 };
        float effectSpeed_{ 1.0f };
        Vector3 effectTarget_{ 0.0f, 0.0, 0.0f };
        Color effectColor_{ 1.0f, 1.0f, 1.0f, 1.0f };
        BitFlag<EffectState> effectState_{};
        WeakPtr<Transform> transform_{ nullptr };
    };
}
