#include <Random.h>
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

auto main() -> int
{
    for (int i = 0; i < 100; i++)
    {
        cout << Random::Instance().rand(0, 1) << "\n";
    }
    //std::ofstream file{ "random.tsv" };
    //for (size_t i = 0; i < 1000 * 1000; ++i)
    //{
    //    Random::Instance().Next();
    //    // Še•ª•z–@‚ÉŠî‚¢‚Ä—”‚ð¶¬
    //    double r1 = Random::Instance().Range(-1.0f, 1.0f);
    //    double r2 = Random::Instance().Range(1.0f, 0.5f);

    //    file << r1 << "\t" << r2 << "\n";
    //}
}
