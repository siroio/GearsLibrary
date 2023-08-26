#include <Random.h>
#include <random>

void Glib::Pcg32Fast::Seed(uint64_t seed)
{
    state = 0;
    Gen();
    state += seed;
    Gen();
}

uint32_t Glib::Pcg32Fast::Gen()
{
    uint64_t oldstate = state;
    state = oldstate * MULTIPLIER + INCREMENT;
    uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << (-static_cast<int>(rot) & 31));
}

bool Glib::Random::Initialize()
{
    generator = std::make_unique<Pcg32Fast>(std::random_device{}());
    return generator != nullptr ? true : false;
}

void Glib::Random::Seed(uint64_t seed)
{
    Instance()->generator->Seed(seed);
}

uint32_t Glib::Random::Next()
{
    uint32_t num = Instance()->generator->Gen();
    return num;
}

float Glib::Random::Nextf()
{
    uint32_t num = Instance()->generator->Gen();
    return static_cast<float>(num * Pcg32Fast::DIVIDE);
}

int Glib::Random::Range(int min, int max)
{
    uint32_t num = Instance()->generator->Gen();
    return (num % (max - min)) + min;
}

float Glib::Random::Range(float min, float max)
{
    uint32_t num = Instance()->generator->Gen();
    return static_cast<float>((min + (max - min) * (num * Pcg32Fast::DIVIDE)));
}

Glib::Pcg32Fast::Pcg32Fast(uint64_t seed)
{
    Seed(seed);
}

uint64_t Glib::Pcg32Fast::Seed() const
{
    return seed.load();
}
