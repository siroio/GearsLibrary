#include <Components/Animator.h>
#include <Components/SkinnedMeshRenderer.h>
#include <GameObject.h>
#include <GameTimer.h>
#include <AnimationManager.h>
#include <Mathf.h>

namespace
{
    auto& s_animationManager = Glib::AnimationManager::Instance();
}

void Glib::Animator::Start()
{
    renderer_ = GameObject()->GetComponent<SkinnedMeshRenderer>();
    if (renderer_.expired()) Destroy();
}

void Glib::Animator::Update()
{
    if (animation_.expired()) return;

    const auto& bones = renderer_->Bones();
    const auto& transforms = renderer_->BoneTransforms();
    auto& matrix = renderer_->BoneMatrix();

    if (matrix.empty()) return;

    // スキニング計算
    for (const auto& bone : bones)
    {
        if (bone.boneIndex >= static_cast<int>(matrix.size())) continue;

        auto keyFrame = animation_->GetKeyFrame(bone.name, currentFrame);
        Vector3 translate = keyFrame.position;
        Quaternion rotation = keyFrame.rotation;
        Vector3 scale = keyFrame.scale;

        // 前のアニメーションをブレンド
        if (!prevAnimation_.expired())
        {
            auto keyFrame = prevAnimation_->GetKeyFrame(bone.name, prevFrame);
            Vector3 prevTranslate = keyFrame.position;
            Quaternion prevRotation = keyFrame.rotation;

            float ratio = elapsedTime / animationBlendTime;
            translate = Vector3::Lerp(prevTranslate, translate, ratio);
            rotation = Quaternion::Slerp(prevRotation, rotation, ratio);
        }

        matrix[bone.boneIndex] =
            Matrix4x4::Translate(-translate) *
            Matrix4x4::Rotate(rotation) * Matrix4x4::Translate(translate) *
            Matrix4x4::Translate(translate);

        // 相対位置計算
        const auto& transform = transforms[bone.boneIndex];
        if (bone.parent != -1) translate -= bones[bone.parent].position;

        transform->LocalPosition(bone.position + translate);
        transform->LocalRotation(rotation);
    }

    float deltaTime = GameTimer::DeltaTime() * animationSpeed;
    elapsedTime += deltaTime;

    currentFrame *= 60.0f + deltaTime;
    if (currentFrame > animation_->EndFrame())
    {
        currentFrame = isLoop ? 0.0f : animation_->EndFrame();
    }
}

bool Glib::Animator::Loop() const
{
    return isLoop;
}

void Glib::Animator::Loop(bool loop)
{
    isLoop = loop;
}

float Glib::Animator::Speed() const
{
    return animationSpeed;
}

void Glib::Animator::Speed(float speed)
{
    animationSpeed = speed;
}

unsigned int Glib::Animator::AnimationClipID() const
{
    return animationClipID_;
}

void Glib::Animator::AnimationClipID(unsigned int id, float offset)
{
    animationClipID_ = id;
    if (elapsedTime > Mathf::EPSILON)
    {
        std::swap(prevAnimation_, animation_);
        prevFrame = currentFrame;
    }

    animation_ = s_animationManager.Animation(id);
    currentFrame = offset;
    elapsedTime = 0.0f;
}
