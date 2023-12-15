#include <string>
#include <mmd/PmxObject.h>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << " <input_file> <output_file>" << std::endl;
    }

    auto inputFile = std::string{ argv[1] };
    auto outputFile = std::string{ argv[2] };

    PmxModel pmx = PmxModel{};
    if (!pmx.LoadModel(inputFile))
    {
        std::cerr << "This file is not supported." << std::endl;
        return -1;
}
    if (!pmx.WriteModel(outputFile))
    {
        std::cerr << "Write error." << std::endl;
        return -1;
    }

    auto path = std::filesystem::path{ outputFile }.lexically_normal();
    path = path.is_absolute() ? path : std::filesystem::absolute(path);
    std::cout << "Output => " << path.remove_filename().generic_string() << std::endl;

}
