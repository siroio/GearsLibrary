#pragma once
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <chrono>

class GameTimer final :
    public Glib::SingletonPtr<GameTimer>,
    public Glib::Internal::Interface::ISystem,
    public Glib::Internal::Function::UpdateOrderSet<Glib::Internal::Order::Update::TIME>
{
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<float>;
    friend Glib::WeakPtr<GameTimer> Glib::SingletonPtr<GameTimer>::Instance();
    GameTimer() = default;

public:
    void Update();

    /**
     * @brief 直前のフレームと今のフレーム間で経過した時間
     * @return 秒
     */
    static float DeltaTime();

    /**
     * @brief タイムスケールの変更
     * @param timeScale
     */
    static void TimeScale(float timeScale = 1.0f);

    /**
     * @brief 直前のフレームと今のフレーム間で経過した時間 *タイムスケールの影響を受けない
     * @return 秒
     */
    static float UnscaledDeltaTime();

private:
    static inline Duration maxDeltaTime_{ 0.1f };
    static inline Duration deltaTime_{ 0.0f };
    static inline Duration unscaledDeltaTime_;
    static inline float timeScale_{ 1.0f };
    TimePoint prevTime_{ Clock::now() };
};
