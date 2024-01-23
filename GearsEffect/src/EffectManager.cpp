#include <EffectManager.h>
#include <Internal/EffekseerManager.h>

namespace
{
    auto s_efkManager = Glib::Internal::Effect::EffekseerManager::Instance();
}

bool Glib::EffectManager::Load(unsigned int id, std::string_view path)
{
    return s_efkManager->Load(id, path);
}
