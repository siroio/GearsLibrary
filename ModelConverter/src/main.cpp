#include <string>
#include <mmd/PmxObject.h>
#include <iostream>

int main(int argc, char* argv[])
{
    //if (argc < 2)
    //{
    //    std::cerr << "Usage: " << argv[0] << " <input_file.pmx> <output_file.pmx>" << std::endl;
    //}

    auto inputFile = std::string{ "C:/Users/rukar/Desktop/初音ミクif v11/初音ミクif.pmx" };
    auto outputFile = std::string{ "ifmiku" };

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
