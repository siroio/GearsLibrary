#ifndef GEAR_RANDOM_H
#define GEAR_RANDOM_H

#include <cstdint>
#include <Singleton.h>

struct PCG_XSH_RR
{
private:
    std::uint64_t state;
    const std::uint64_t increment;

public:
    explicit PCG_XSH_RR(std::uint64_t seed = 0, std::uint64_t inc = 6364136223846793005ull);
    void Seed(std::uint64_t seed);
    std::uint32_t NextInt();
    double NextDouble();
};

class Random : public Singleton<Random>
{
private:
    PCG_XSH_RR pcg{};

public:
    void Init();
    void Seed(std::uint64_t seed);
    int Next();
    float Nextf();
    int Range(int min, int max);
    float Range(float min, float max);
};
#endif // !GEAR_RANDOM_H
