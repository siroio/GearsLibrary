#include <AnimationManager.h>
#include <unordered_map>

namespace
{
    std::unordered_map<unsigned int, std::shared_ptr<Glib::AnimationClip>> s_animations;
}

bool Glib::AnimationManager::Load(unsigned int id, std::string_view filePath)
{
    auto animation = std::make_shared<AnimationClip>();
    if (animation->Load(filePath)) return false;
    s_animations.emplace(animation);
    return true;
}

Glib::WeakPtr<Glib::AnimationClip> Glib::AnimationManager::Animation(unsigned int id)
{
    if (!s_animations.contains(id)) return Glib::WeakPtr<AnimationClip>{ nullptr };
    return Glib::WeakPtr<AnimationClip>{ s_animations.at(id) };
}
