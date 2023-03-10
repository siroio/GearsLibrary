#ifndef GEAR_RANDOM_H
#define GEAR_RANDOM_H

#include<cstdint>

class PCG_XSH_RR
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

class Random
{
private:
    static PCG_XSH_RR pcg;

public:
    static void Seed(std::uint64_t seed);
    static int Next();
    static float Nextf();
    static int Range(int min, int max);
    static float Range(float min, float max);
};
#endif // !GEAR_RANDOM_H
