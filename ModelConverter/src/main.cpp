#include <string>
#include <mmd/PmxObject.h>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << " <input_file> <output_file>" << std::endl;
    }

#if _DEBUG | DEBUG
    auto inputFile = std::string{ "C:/Users/rukar/Desktop/MikuMikuDance_v932x64/�����~�Nif v11/�����~�Nif.pmx" };
    auto outputFile = std::string{ "C:/Users/rukar/Desktop/MikuMikuDance_v932x64/�����~�Nif v11/�����~�Nif" };
#else
    auto inputFile = std::string{ argv[1] };
    auto outputFile = std::string{ argv[2] };
#endif // DEBUG_ | DEBUG



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
