#include <Internal/ComponentManager.h>
#include <GameTimer.h>

namespace
{
    float s_accumulatedTime{ 0.0f };
}

void Glib::Internal::ComponentManager::Update()
{
    normalFunction_.Update();
    normalFunction_.ExecuteClear(ComponentFunctionType::Start);

    const float deltaTime{ GameTimer::DeltaTime() };
    s_accumulatedTime = std::fminf(s_accumulatedTime + deltaTime, GameTimer::MaximumAllowedTimestep());

    // fixedUpdate‚ð—­‚Ü‚Á‚½ŽžŠÔ•ªi‚ß‚é
    while (s_accumulatedTime > GameTimer::FixedTimeStep())
    {
        normalFunction_.Execute(ComponentFunctionType::FixedUpdate);
        s_accumulatedTime -= GameTimer::FixedTimeStep();
    }

    normalFunction_.Execute(ComponentFunctionType::Update);
    normalFunction_.Execute(ComponentFunctionType::LateUpdate);
}

void Glib::Internal::ComponentManager::Finalize()
{
    normalFunction_.Clear();
    eventFunction_.clear();
}
