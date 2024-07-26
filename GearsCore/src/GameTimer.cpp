#include <GameTimer.h>
#include <Mathf.h>

void Glib::GameTimer::Update()
{
    const TimePoint now = Clock::now();
    const Duration deltaTime = now - prevTime_;
    unscaledDeltaTime_ = std::min(deltaTime, maxDeltaTime_);
    deltaTime_ = unscaledDeltaTime_ * timeScale_;
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
    return FixedTimeStep();
}

float Glib::GameTimer::FixedTimeStep()
{
    return fixedTimeStep_ * timeScale_;
}

void Glib::GameTimer::FixedTimeStep(float timeStep)
{
    fixedTimeStep_ = Mathf::Max(timeStep, 0.0f);
}

float Glib::GameTimer::MaximumAllowedTimeStep()
{
    return maximumAllowedTimeStep_.count();
}

void Glib::GameTimer::TimeScale(float timeScale)
{
    timeScale_ = Mathf::Max(timeScale, 0.0f);
}

void Glib::GameTimer::MaximumAllowedTimeStep(float maximumAllowedTimeStep)
{
    maximumAllowedTimeStep_ = Duration{ maximumAllowedTimeStep };
}

float Glib::GameTimer::UnscaledDeltaTime()
{
    return unscaledDeltaTime_.count();
}

float Glib::GameTimer::FixedUnscaledDeltaTime()
{
    return fixedTimeStep_;
}
