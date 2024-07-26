#include <AnimationClip.h>
#include <GLAnimation.h>
#include <algorithm>
#include <ranges>
#include <Mathf.h>

Matrix4x4 Glib::AnimationClip::KeyFrame::Matrix() const
{
    return Matrix4x4::TRS(position, rotation, scale);
}

bool Glib::AnimationClip::Load(std::string_view fileName)
{
    GLAnimation animation{};
    if (!animation.ReadFile(fileName)) return false;

    for (const auto& motion : animation.Motions())
    {
        AnimationClip::KeyFrame keyFrame{};
        keyFrame.frameNo = static_cast<float>(motion.frameNo);
        std::ranges::copy(motion.translation, keyFrame.position.xyz.begin());
        std::ranges::copy(motion.rotation, keyFrame.rotation.q.begin());
        std::ranges::copy(motion.scale, keyFrame.scale.xyz.begin());
        endFrame_ = std::max(endFrame_, keyFrame.frameNo);
        keyframes_[motion.boneName].push_back(std::move(keyFrame));
    }

    // フレームを昇順にソート
    for (auto& [bone, frame] : keyframes_)
    {
        std::ranges::sort(frame, [](const KeyFrame& lhs, const KeyFrame& rhs)
        {
            return lhs.frameNo < rhs.frameNo;
        });
        frame.shrink_to_fit();
    }
    return true;
}

Glib::AnimationClip::KeyFrame Glib::AnimationClip::GetKeyFrame(const std::string& boneName, float frameNo) const
{
    // キーフレームを検索
    const auto& it = keyframes_.find(boneName);

    if (it == keyframes_.end())
    {
        KeyFrame keyframe{};
        keyframe.frameNo = frameNo;
        keyframe.position = Vector3::Zero();
        keyframe.rotation = Quaternion::Identity();
        keyframe.scale = Vector3::One();
        return keyframe;
    }

    const auto& keyframes = it->second;
    const auto& [start, end] = SearchKeyFrame(keyframes, frameNo);

    // 一致したらそのまま返す
    if (start == end) return keyframes.at(start);

    const auto& startFrame = keyframes.at(start);
    const auto& endFrame = keyframes.at(end);

    // フレーム補間
    KeyFrame interpolate{};
    const float t = Mathf::Remap01(frameNo, startFrame.frameNo, endFrame.frameNo);
    interpolate.frameNo = frameNo;
    interpolate.position = Vector3::Lerp(startFrame.position, endFrame.position, t);
    interpolate.rotation = Quaternion::Slerp(startFrame.rotation, endFrame.rotation, t);
    interpolate.scale = Vector3::Lerp(startFrame.scale, endFrame.scale, t);
    return interpolate;
}

float Glib::AnimationClip::EndFrame() const
{
    return endFrame_;
}

std::tuple<int, int> Glib::AnimationClip::SearchKeyFrame(const KeyFrames& keys, float frameNo) const
{
    // 2分探索でフレームを探索

    int start = -1;
    int end = static_cast<int>(keys.size());

    if (frameNo <= keys.front().frameNo)
    {
        return { 0, 0 };
    }

    if (frameNo >= keys.back().frameNo)
    {
        return { end - 1, end - 1 };
    }

    while (Mathf::Abs(end - start) > 1)
    {
        int mid = (end + start) / 2;

        if (keys.at(mid).frameNo <= frameNo)
        {
            start = mid;
        }
        else
        {
            end = mid;
        }
    }

    return { start, end };
}
