#include <GameTimer.h>

void GameTimer::Update()
{
    timePoint now = clock::now();
    duration unscaledDeltaTime = now - prevTime_;
    if (unscaledDeltaTime > maxDeltaTime_) unscaledDeltaTime_ = maxDeltaTime_;
    deltaTime_ = unscaledDeltaTime * timeScale_;
    prevTime_ = now;
}

float GameTimer::DeltaTime()
{
    return deltaTime_.count();
}

void GameTimer::TimeScale(float timeScale)
{
    if (timeScale < 0.0f) timeScale = 0.0f;
}

float GameTimer::UnscaledDeltaTime()
{
    return unscaledDeltaTime_.count();
}
