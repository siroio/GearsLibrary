#include <Internal/ComponentManager.h>

void Glib::Internal::ComponentManager::Update()
{
    normalFunction_.Update();
    normalFunction_.ExecuteClear(ComponentFunctionType::Start);
    normalFunction_.Execute(ComponentFunctionType::Update);
    normalFunction_.Execute(ComponentFunctionType::LateUpdate);
}

void Glib::Internal::ComponentManager::FixedUpdate()
{
    normalFunction_.Update();
    normalFunction_.Execute(ComponentFunctionType::FixedUpdate);
}

void Glib::Internal::ComponentManager::Finalize()
{
    normalFunction_.Clear();
    eventFunction_.clear();
}
