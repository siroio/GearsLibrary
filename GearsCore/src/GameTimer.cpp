#include <GameTimer.h>

void GameTimer::Update()
{
    TimePoint now = Clock::now();
    Duration unscaledDeltaTime = now - prevTime_;
    if (unscaledDeltaTime > maxDeltaTime_) unscaledDeltaTime_ = maxDeltaTime_;
    deltaTime_ = unscaledDeltaTime * timeScale_;
    prevTime_ = now;
}

void GameTimer::FixedUpdate()
{
    deltaTime_ = std::chrono::duration_cast<Duration>(Duration{ fixedTimestep_ });
}

float GameTimer::DeltaTime()
{
    return deltaTime_.count();
}

float GameTimer::FixedDeltaTime()
{
    return fixedTimestep_;
}

float GameTimer::FixedTimeStep()
{
    return fixedTimestep_;
}

void GameTimer::FixedTimeStep(float timeStep)
{
    fixedTimestep_ = timeStep;
}

float GameTimer::MaximumAllowedTimestep()
{
    return maximumAllowedTimestep_.count();
}

void GameTimer::TimeScale(float timeScale)
{
    timeScale_ = timeScale < 0.0f ? 0.0f : timeScale;
}

void GameTimer::MaximumAllowedTimestep(float maximumAllowedTimestep)
{
    maximumAllowedTimestep_ = std::chrono::duration_cast<Duration>(Duration{ maximumAllowedTimestep });;
}

float GameTimer::UnscaledDeltaTime()
{
    return unscaledDeltaTime_.count();
}
