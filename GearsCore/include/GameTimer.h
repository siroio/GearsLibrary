#ifndef GEARS_GAME_TIMER_H
#define GEARS_GAME_TIMER_H

#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <chrono>

class GameTimer final :
    public GLib::Utility::SingletonPtr<GameTimer>,
    public GLib::Internal::Interface::ISystem,
    public GLib::Internal::Function::UpdateOrderSet<GLib::Constant::Order::Update::TIME>
{

    using clock = std::chrono::high_resolution_clock;
    using timePoint = clock::time_point;
    using duration = std::chrono::duration<float>;

private:
    GameTimer() = default;

public:
    void Update();
    static float DeltaTime();
    static void TimeScale(float timeScale);
    static float UnscaledDeltaTime();

private:
    static inline duration maxDeltaTime_{ 0.1f };
    static inline duration deltaTime_{ 0.0f };
    static inline duration unscaledDeltaTime_;
    static inline float timeScale_{ 1.0f };
    timePoint prevTime_{ clock::now() };
};

#endif // !GEARS_GAME_TIMER_H
