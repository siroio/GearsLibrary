#include <AnimationClip.h>
#include <algorithm>
#include <ranges>
#include <Mathf.h>

Matrix4x4 Glib::AnimationClip::KeyFrame::Matrix() const
{
    return Matrix4x4::TRS(position, rotation, scale);
}

bool Glib::AnimationClip::Load(std::string_view fileName)
{
    return false;
}

Glib::AnimationClip::KeyFrame Glib::AnimationClip::GetKeyFrame(std::string_view boneName, float frameNo) const
{
    // キーフレームを検索
    if (!keyframes_.contains(boneName.data()))
    {
        KeyFrame keyframe{};
        keyframe.frameNo = frameNo;
        keyframe.position = Vector3::Zero();
        keyframe.rotation = Quaternion::Identity();
        keyframe.scale = Vector3::One();
        return keyframe;
    }

    const auto& keyframes = keyframes_.at(boneName.data());
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
    float result{ 0.0f };
    for (const auto& key : keyframes_)
    {
        result = std::max(result, key.second.back().frameNo);
    }
    return result;
}

std::pair<int, int> Glib::AnimationClip::SearchKeyFrame(const KeyFrames& keys, float frameNo) const
{
    auto it = std::lower_bound(keys.begin(), keys.end(), frameNo, [](const KeyFrame& key, float frame)
    {
        return key.frameNo < frame;
    });

    if (it == keys.begin())
    {
        return std::pair<int, int>{ 0, 0 };
    }

    if (it == keys.end())
    {
        int last_frame = static_cast<int>(keys.size() - 1);
        return std::pair<int, int>{ last_frame, last_frame };
    }

    int end = static_cast<int>(std::distance(keys.begin(), it));
    int start = end - 1;
    return std::pair<int, int>{start, end};
}
