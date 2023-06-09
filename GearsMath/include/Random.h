#ifndef GEARS_RANDOM_H
#define GEARS_RANDOM_H
#include <Singleton.h>
#include <Mathf.h>
#include <random>

namespace
{
    constexpr uint64_t MULTIPLIER = 6364136223846793005u;
    constexpr uint64_t INCREMENT = 1442695040888963407u;
    constexpr float DIVIDE = static_cast<float>(1.0f / 4294967295.0);
}

/**
 * @brief Permuted congruential generator 乱数生成器
 */
class Pcg32Fast
{
private:
    uint64_t state;

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
};

/**
 * @brief 乱数生成 ラッパークラス
 */
class Random : public GLib::Singleton<Random>
{
private:
    Pcg32Fast rand{ std::random_device{}() };

public:
    void Seed(uint64_t seed);
    uint32_t Next();
    float Nextf();
    int Range(int min, int max);
    float Range(float min, float max);
};
#endif // !GEARS_RANDOM_H
