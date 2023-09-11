#pragma once
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <chrono>

namespace Glib
{
    class GameTimer final :
        public SingletonPtr<GameTimer>,
        public Internal::Interface::ISystem,
        public Internal::Function::UpdateOrderSet<Internal::Order::Update::TIME>
    {
    private:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;
        using Duration = std::chrono::duration<float>;
        friend WeakPtr<GameTimer> SingletonPtr<GameTimer>::Instance();
        GameTimer() = default;

    public:
        void Update();

        /**
         * @brief 直前のフレームと今のフレーム間で経過した時間
         * @return 秒
         */
        static float DeltaTime();

        /**
         * @brief 固定フレームレートの更新を実行するインターバル
         * @return 秒
        */
        static float FixedDeltaTime();

        /**
         * @brief タイムステップを取得
         * @param timeStep
        */
        static float FixedTimeStep();

        /**
         * @brief タイムステップを設定
         * @param timeStep
        */
        static void FixedTimeStep(float timeStep);

        /**
         * @brief タイムテップの上限値取得
         * @param maximumAllowedTimestep
        */
        static float MaximumAllowedTimeStep();

        /**
         * @brief タイムテップの上限値設定
         * @param maximumAllowedTimeStep
        */
        static void MaximumAllowedTimeStep(float maximumAllowedTimeStep);

        /**
         * @brief タイムスケールの変更
         * @param timeScale
         */
        static void TimeScale(float timeScale = 1.0f);

        /**
         * @brief 直前のフレームと今のフレーム間で経過した時間
         * @brief タイムスケールの影響を受けない
         * @return 秒
         */
        static float UnscaledDeltaTime();

    private:
        static inline Duration maxDeltaTime_{ 0.1f };
        static inline Duration deltaTime_{ 0.0f };
        static inline Duration unscaledDeltaTime_{ 1.0f };
        static inline Duration maximumAllowedTimeStep_{ 0.3333333f };
        static inline float timeScale_{ 1.0f };
        static inline float fixedTimeStep_{ 0.02f };
        TimePoint prevTime_{ Clock::now() };
    };
}
