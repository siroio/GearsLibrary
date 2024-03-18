#pragma once
#include <Component.h>
#include <WeakPtr.h>
#include <AnimationClip.h>
#include <BitFlag.h>
#include <Internal/SkinnedRenderer.h>

namespace Glib
{
    class Animator : public Component
    {
    public:
        void Start();
        void Update();

        bool Loop() const;
        void Loop(bool loop);

        float Speed() const;
        void Speed(float speed);

        bool Resume() const;
        void Resume(bool enable);

        unsigned int AnimationID() const;
        void AnimationID(unsigned int id, float offset = 0.0f);

        virtual void OnGUI() override;

    private:
        WeakPtr<Internal::SkinnedRenderer> renderer_{ nullptr };
        WeakPtr<AnimationClip> animation_{ nullptr };
        WeakPtr<AnimationClip> prevAnimation_{ nullptr };
        unsigned int clipID_{ INT_MAX };
        bool isLoop_{ false };
        bool isResume_{ false };
        float animationSpeed_{ 1.0f };
        float animationFrameRate_{ 60.0f };
        float currentFrame_{ 0.0f };
        float prevFrame_{ 0.0f };
        float animationBlendTime_{ 0.1f };
        float elapsedTime_{ 0.0f };
    };
}
