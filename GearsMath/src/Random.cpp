#include <Random.h>
#include <functional>
#include <limits>

void Random::Seed(uint64_t seed)
{
    rand.Seed(seed);
}

uint32_t Random::Next()
{
    return rand();
}

float Random::Nextf()
{
    return static_cast<float>(rand() * DIVIDE);
}

int Random::Range(int min, int max)
{
    return rand();
}

float Random::Range(float min, float max)
{
    return static_cast<float>((min + (max - min) * (rand() * DIVIDE)));
}
