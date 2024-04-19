#include <Components/Animator.h>
#include <Components/SkinnedMeshRenderer.h>
#include <GameObject.h>
#include <GameTimer.h>
#include <AnimationManager.h>
#include <Mathf.h>
#include <Debugger.h>
#include <GLGUI.h>
#include <Vector4.h>

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
    const auto& bones = renderer_->Bones();
    const auto& transforms = renderer_->BoneTransforms();
    auto& matrix = renderer_->BoneMatrix();

    if (matrix.empty()) return;
    matrix.fill(Matrix4x4::Identity());

    // スキニング計算
    for (const auto& bone : bones)
    {
        Vector3 translation{};
        Quaternion rotation{};
        Vector3 scale{};

        if (!animation_.expired())
        {
            const auto& keyFrame = animation_->GetKeyFrame(bone.name, currentFrame_);
            translation = keyFrame.position;
            rotation = keyFrame.rotation;
            scale = keyFrame.scale;

            // 前のアニメーションをブレンド
            if (!prevAnimation_.expired())
            {
                const auto& blend = prevAnimation_->GetKeyFrame(bone.name, prevFrame_);
                float ratio = elapsedTime_ / animationBlendTime_;
                translation = Vector3::Lerp(blend.position, translation, ratio);
                rotation = Quaternion::Slerp(blend.rotation, rotation, ratio);
            }

            matrix[bone.boneIndex]
                = Matrix4x4::Translate(-bone.position)
                * Matrix4x4::Rotate(rotation) * Matrix4x4::Translate(translation)
                * Matrix4x4::Translate(bone.position);
        }

        // 相対位置計算
        const auto& transform = transforms.at(bone.boneIndex);
        if (bone.parent != -1)
        {
            translation -= bones[bone.parent].position;
        }

        transform->LocalPosition(bone.position + translation);
        transform->LocalRotation(rotation);
    }

    if (animation_.expired() || isResume_) return;
    float deltaTime = GameTimer::DeltaTime() * animationSpeed_;
    elapsedTime_ += deltaTime;
    currentFrame_ += animationFrameRate_ * deltaTime;

    if (currentFrame_ > animation_->EndFrame())
    {
        currentFrame_ = isLoop_ ? 0.0f : animation_->EndFrame();
    }
}

bool Glib::Animator::Loop() const
{
    return isLoop_;
}

void Glib::Animator::Loop(bool loop)
{
    isLoop_ = loop;
}

float Glib::Animator::Speed() const
{
    return animationSpeed_;
}

void Glib::Animator::Speed(float speed)
{
    animationSpeed_ = speed;
}

float Glib::Animator::BlendTime() const
{
    return animationBlendTime_;
}

void Glib::Animator::BlendTime(float blendTime)
{
    animationBlendTime_ = blendTime;
}

bool Glib::Animator::Pause() const
{
    return isResume_;
}

void Glib::Animator::Pause(bool enable)
{
    isResume_ = enable;
}

unsigned int Glib::Animator::AnimationID() const
{
    return clipID_;
}

void Glib::Animator::AnimationID(unsigned int id, float offset)
{
    clipID_ = id;
    if (elapsedTime_ > Mathf::EPSILON)
    {
        std::swap(prevAnimation_, animation_);
        prevFrame_ = currentFrame_;
    }

    animation_ = s_animationManager.Animation(id);
    currentFrame_ = offset;
    elapsedTime_ = 0.0f;
}

void Glib::Animator::OnGUI()
{
    Component::OnGUI();
    int clipID = clipID_;
    if (GLGUI::InputInt("ClipID", &clipID))
    {
        AnimationID(clipID);
    }
    GLGUI::CheckBox("Loop", &isLoop_);
    GLGUI::CheckBox("Resume", &isResume_);
    GLGUI::DragFloat("Speed", &animationSpeed_, 0.01f, Mathf::EPSILON);
    GLGUI::SliderFloat("BlendTime", &animationBlendTime_, 0.0f, 1.0f);
    GLGUI::Separator();
    GLGUI::InputFloat("Frame", &currentFrame_);
}
