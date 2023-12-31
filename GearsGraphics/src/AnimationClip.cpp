#include <AnimationClip.h>
#include <algorithm>
#include <ranges>
#include <Mathf.h>
#include <GLAnimation.h>
#include <iostream>

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
        const auto& frame = motion.frame;
        keyFrame.frameNo = static_cast<float>(frame.frameNo);
        std::copy(std::begin(frame.translation), std::end(frame.translation), &keyFrame.position[0]);
        std::copy(std::begin(frame.rotation), std::end(frame.rotation), &keyFrame.rotation[0]);
        std::copy(std::begin(frame.scale), std::end(frame.scale), &keyFrame.scale[0]);
        keyframes_[motion.info.boneName].push_back(keyFrame);
        endFrame_ = std::max(endFrame_, keyFrame.frameNo);
    }

    // メモリをsizeに合わせる
    for (auto& [bone, frame] : keyframes_)
    {
        frame.shrink_to_fit();
    }
    return true;
}

Glib::AnimationClip::KeyFrame Glib::AnimationClip::GetKeyFrame(const std::string& boneName, float frameNo) const
{
    // キーフレームを検索
    if (!keyframes_.contains(boneName))
    {
        KeyFrame keyframe{};
        keyframe.frameNo = frameNo;
        keyframe.position = Vector3::Zero();
        keyframe.rotation = Quaternion::Identity();
        keyframe.scale = Vector3::One();
        return keyframe;
    }

    const auto& keyframes = keyframes_.at(boneName);
    auto segment = SearchKeyFrame(keyframes, frameNo);

    // 一致したらそのまま返す
    if (segment.first == segment.second) return keyframes.at(segment.first);

    const auto& start = keyframes.at(segment.first);
    const auto& end = keyframes.at(segment.second);

    // フレーム補間
    KeyFrame interpolate{};
    const float t = Mathf::Remap01(frameNo, start.frameNo, end.frameNo);
    interpolate.frameNo = frameNo;
    interpolate.position = Vector3::Lerp(start.position, end.position, t);
    interpolate.rotation = Quaternion::Slerp(start.rotation, end.rotation, t);
    interpolate.scale = Vector3::Lerp(start.scale, end.scale, t);
    return interpolate;
}

float Glib::AnimationClip::EndFrame() const
{
    return endFrame_;
}

std::pair<int, int> Glib::AnimationClip::SearchKeyFrame(const KeyFrames& keys, float frameNo) const
{
    int start = 0;
    int end = static_cast<int>(keys.size() - 1);

    if (frameNo <= keys.at(start).frameNo)
    {
        return std::pair<int, int>{ 0, 0 };
    }

    if (frameNo >= keys.at(end).frameNo)
    {
        return std::pair<int, int>{ end, end };
    }

    while ((start + 1) < end)
    {
        int mid = (start + end) / 2;
        if (keys.at(mid).frameNo == frameNo)
        {
            return std::pair<int, int>{ mid, mid };
        }
        else if (keys.at(mid).frameNo < frameNo)
        {
            start = mid;
        }
        else
        {
            end = mid;
        }
    }

    return std::pair<int, int>{ start, end };
}
