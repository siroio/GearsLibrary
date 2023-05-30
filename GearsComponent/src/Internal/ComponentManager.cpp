#include <Internal/ComponentManager.h>

void GLib::Internal::ComponentManager::Update()
{
    normalFunction_.Update();
    normalFunction_.ExecuteClear(ComponentFunctionType::Start);
    normalFunction_.Execute(ComponentFunctionType::Update);
    normalFunction_.Execute(ComponentFunctionType::LateUpdate);
}

void GLib::Internal::ComponentManager::Finalize()
{
    normalFunction_.Clear();
    eventFunction_.clear();
}
