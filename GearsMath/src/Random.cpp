#include <Random.h>
#include <random>

bool Glib::Random::Initialize()
{
    generator = std::make_unique<Pcg32Fast>();
    generator->Seed(std::random_device{}());
}

void Glib::Random::Seed(uint64_t seed)
{
    generator->Seed(seed);
}

uint32_t Glib::Random::Next()
{
    return generator->Gen();
}

float Glib::Random::Nextf()
{
    return static_cast<float>(generator->Gen() * Pcg32Fast::DIVIDE);
}

int Glib::Random::Range(int min, int max)
{
    return (generator->Gen() % (max - min)) + min;
}

float Glib::Random::Range(float min, float max)
{
    return static_cast<float>((min + (max - min) * (generator->Gen() * Pcg32Fast::DIVIDE)));
}
