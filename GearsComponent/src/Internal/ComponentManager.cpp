#include <Internal/ComponentManager.h>
#include <GameTimer.h>

namespace
{
    float accumulatedTime{ 0.0f };
}

void Glib::Internal::ComponentManager::Update()
{
    normalFunction_.Update();
    normalFunction_.ExecuteClear(ComponentFunctionType::Start);

    const float deltaTime{ GameTimer::DeltaTime() };
    accumulatedTime += deltaTime;

    // fixedUpdateを溜まった時間分進める
    while (accumulatedTime > 0.0f)
    {
        normalFunction_.Execute(ComponentFunctionType::FixedUpdate);
        accumulatedTime -= deltaTime <= GameTimer::FixedTimeStep() ? GameTimer::FixedTimeStep() : GameTimer::MaximumAllowedTimestep();
    }

    normalFunction_.Execute(ComponentFunctionType::Update);
    normalFunction_.Execute(ComponentFunctionType::LateUpdate);
}

void Glib::Internal::ComponentManager::Finalize()
{
    normalFunction_.Clear();
    eventFunction_.clear();
}
