#include <CSVLoader.h>

#include <vector>
#include <fstream>
#include <StringUtility.h>

CSVLoader::CSVLoader(std::string_view path)
{
    this->path = path;
}

CSVLoader::~CSVLoader()
{}

std::vector<std::vector<std::string>> CSVLoader::Load() noexcept(false)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file: " + path);
    }

    strings result;
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> row = GLib::Utility::Split(line, ',');
        result.push_back(row);
    }

    return result;
}

