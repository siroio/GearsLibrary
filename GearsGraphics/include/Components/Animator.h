#pragma once
#include <Component.h>
#include <WeakPtr.h>
#include <AnimationClip.h>
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

        unsigned int AnimationClipID() const;
        void AnimationClipID(unsigned int id, float offset = 0.0f);

    private:
        WeakPtr<Internal::SkinnedRenderer> renderer_{ nullptr };
        WeakPtr<AnimationClip> animation_{ nullptr };
        WeakPtr<AnimationClip> prevAnimation_{ nullptr };
        unsigned int animationClipID_{ INT_MAX };
        bool isLoop{ false };
        float animationSpeed{ 1.0f };
        float currentFrame{ 0.0f };
        float prevFrame{ 0.0f };
        float animationBlendTime{ 0.1f };
        float elapsedTime{ 0.0f };
    };
}
