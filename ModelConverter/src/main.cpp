#include <string>
#include <iostream>
#include <filesystem>
#include <Converter.h>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << " <input_file> <output_file>" << std::endl;
    }

    auto inputFile = fs::path{ argv[1] }.lexically_normal();
    auto outputFile = fs::path{ argv[2] }.lexically_normal();
    outputFile = outputFile.is_absolute() ? outputFile : std::filesystem::absolute(outputFile);

    try
    {
        if (!fs::exists(inputFile)) throw std::runtime_error{ "File is not exist." };
        Converter::Initialize();
        auto inputExt = inputFile.extension().generic_string();
        std::erase(inputExt, '.'); // erase extension "."
        auto converter = Converter::Create(inputExt);
        if (!converter->LoadFile(inputFile.generic_string()))
        {
            throw std::runtime_error{ "Convert Failed." };
            return -1;
        }

        if (!converter->WriteFile(outputFile.generic_string()))
        {
            throw std::runtime_error{ "File Write Failed." };
            return -1;
        }

        std::cout << "OUTPUT DIR : " << outputFile.generic_string() << std::endl;
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << error.what() << std::endl;
        return -1;
    }
}
