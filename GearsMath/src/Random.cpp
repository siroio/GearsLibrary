#include "Random.h"
#include <random>
#include <limits>

PCG_XSH_RR Random::pcg;

PCG_XSH_RR::PCG_XSH_RR(std::uint64_t seed, std::uint64_t inc)
    : state{ 0ull }, increment{ inc | 1ull }
{
    if (seed == 0)
    {
        seed = std::random_device{}();
    }
    NextInt();
    state += seed;
    NextInt();
}

void PCG_XSH_RR::Seed(std::uint64_t seed)
{
    state = 0ull;
    NextInt();
    state += seed;
    NextInt();
}

void Random::Seed(std::uint64_t seed)
{
    pcg.Seed(seed);
}

std::uint32_t PCG_XSH_RR::NextInt()
{
    std::uint64_t oldState = state;
    state = oldState * increment + increment;
    std::uint32_t xorshifted = static_cast<std::uint32_t>((oldState >> 18) ^ oldState) >> 27;
    int rot = static_cast<int>(oldState >> 59);
    return (xorshifted >> rot) | (xorshifted << (-rot & 31));
}

double PCG_XSH_RR::NextDouble()
{
    return static_cast<double>(NextInt()) / std::numeric_limits<std::uint32_t>::max();
}

int Random::Next()
{
    return static_cast<int>(pcg.NextInt());
}

float Random::Nextf()
{
    return static_cast<float>(pcg.NextDouble());
}

int Random::Range(int min, int max)
{
    return (pcg.NextInt() % (max - min)) + min;
}

float Random::Range(float min, float max)
{
    return static_cast<float>((min + (max - min) * pcg.NextDouble()));
}