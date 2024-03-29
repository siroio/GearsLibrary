﻿#include <GameTimer.h>

void Glib::GameTimer::Update()
{
    const auto now = Clock::now();
    const Duration unscaledDeltaTime = now - prevTime_;
    if (unscaledDeltaTime > maxDeltaTime_) unscaledDeltaTime_ = maxDeltaTime_;
    deltaTime_ = unscaledDeltaTime * timeScale_;
    prevTime_ = now;
}

float Glib::GameTimer::Now()
{
    auto now = Clock::now().time_since_epoch();
    auto time = std::chrono::duration_cast<Duration>(now);
    return time.count();
}

float Glib::GameTimer::DeltaTime()
{
    return deltaTime_.count();
}

float Glib::GameTimer::FixedDeltaTime()
{
    return fixedTimeStep_ * timeScale_;
}

float Glib::GameTimer::FixedTimeStep()
{
    return fixedTimeStep_;
}

void Glib::GameTimer::FixedTimeStep(float timeStep)
{
    fixedTimeStep_ = std::max(timeStep, 0.0f);
}

float Glib::GameTimer::MaximumAllowedTimeStep()
{
    return maximumAllowedTimeStep_.count();
}

void Glib::GameTimer::TimeScale(float timeScale)
{
    timeScale_ = std::max(timeScale, 0.0f);
}

void Glib::GameTimer::MaximumAllowedTimeStep(float maximumAllowedTimeStep)
{
    maximumAllowedTimeStep_ = std::chrono::duration_cast<Duration>(Duration{ maximumAllowedTimeStep });
}

float Glib::GameTimer::UnscaledDeltaTime()
{
    return unscaledDeltaTime_.count();
}

float Glib::GameTimer::FixedUnscaledDeltaTime()
{
    return fixedTimeStep_;
}
