#include <string>
#include <mmd/PmxObject.h>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << " <input_file> <output_file>" << std::endl;
    }

    auto inputFile = std::string{ argv[0] };
    auto outputFile = std::string{ argv[1] };

    PmxModel pmx = PmxModel{};
    if (!pmx.LoadModel(inputFile))
    {
        std::cerr << "This file is not supported." << std::endl;
    }
    if (!pmx.WriteModel(outputFile))
    {
        std::cerr << "Write error." << std::endl;
    }
    return 0;
}
