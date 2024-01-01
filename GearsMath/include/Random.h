#pragma once
#include <atomic>
#include <memory>
#include <Singleton.h>
#include <Internal/ISystem.h>
#include <Mathf.h>

namespace Glib
{
    /**
     * @brief Permuted Congruential Generator 疑似乱数生成器
     */
    class Pcg32Fast
    {
    public:
        explicit Pcg32Fast(uint64_t seed);

        /**
         * @brief 現在のシードを取得
         */
        uint64_t Seed() const;

        /**
         * @brief シードを設定
         */
        void Seed(uint64_t seed = 0x853c49e6748fea9bull);

        /**
         * @brief 生成
         */
        uint32_t Gen();

    public:
        static constexpr uint64_t MULTIPLIER = 6364136223846793005u;
        static constexpr uint64_t INCREMENT = 1442695040888963407u;
        static constexpr float DIVIDE = static_cast<float>(1.0f / 4294967295.0);

    private:
        std::atomic_uint64_t seed;
        std::atomic_uint64_t state;
    };

    /**
     * @brief 乱数生成 ラッパークラス
     */
    class Random :
        public Internal::Interface::ISystem,
        public SingletonPtr<Random>
    {
        Random() = default;
        friend WeakPtr<Random> SingletonPtr<Random>::Instance();

    public:

        /**
         * @brief 初期化
         */
        bool Initialize();

        /**
         * @brief シード値の設定
         * @param seed
         */
        static void Seed(uint64_t seed);

        /**
         * @brief 乱数を生成
         * @return
         */
        static uint32_t Next();

        /**
         * @brief 小数点の乱数を生成
         * @return
         */
        static float Nextf();

        /**
         * @brief min ~ max範囲で乱数を生成
         * @param min
         * @param max
         * @return
         */
        static int Range(int min, int max);

        /**
         * @brief min ~ max範囲で小数点の乱数を生成
         * @param min
         * @param max
         * @return
         */
        static float Range(float min, float max);

    private:
        static inline std::unique_ptr<Pcg32Fast> generator;
    };
}

