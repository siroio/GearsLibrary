#pragma once
#include <Singleton.h>
#include <Mathf.h>
#include <random>

/**
 * @brief Permuted Congruential Generator 乱数生成器
 */
class Pcg32Fast
{
public:
    constexpr explicit Pcg32Fast(uint64_t seed)
    {
        Seed(seed);
    }

    constexpr void Seed(uint64_t seed = 0x853c49e6748fea9bull)
    {
        state = 0;
        (*this)();
        state += seed;
        (*this)();
    }

    constexpr uint32_t operator ()()
    {
        uint64_t oldstate = state;
        state = oldstate * MULTIPLIER + INCREMENT;
        uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
        uint32_t rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << (-static_cast<int>(rot) & 31));
    }

public:
    static inline uint64_t MULTIPLIER = 6364136223846793005u;
    static inline uint64_t INCREMENT = 1442695040888963407u;
    static inline float DIVIDE = static_cast<float>(1.0f / 4294967295.0);

private:
    uint64_t state;
};

/**
 * @brief 乱数生成 ラッパークラス
 */
class Random : public Glib::Singleton<Random>
{
private:
    Pcg32Fast rand{ std::random_device{}() };

public:

    /**
     * @brief シード値の設定
     * @param seed
     */
    void Seed(uint64_t seed);

    /**
     * @brief 乱数を生成
     * @return
     */
    uint32_t Next();

    /**
     * @brief 小数点の乱数を生成
     * @return
     */
    float Nextf();

    /**
     * @brief min ~ max範囲で乱数を生成
     * @param min
     * @param max
     * @return
     */
    int Range(int min, int max);

    /**
     * @brief min ~ max範囲で小数点の乱数を生成
     * @param min
     * @param max
     * @return
     */
    float Range(float min, float max);
};
