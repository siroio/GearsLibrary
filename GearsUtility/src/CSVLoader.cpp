#include <CSVLoader.h>

#include <vector>
#include <fstream>
#include <string>
#include <StringUtility.h>

CSVLoader::CSVLoader(const char* path)
{
    this->path = path;
}

CSVLoader::~CSVLoader()
{}

std::vector<std::vector<std::string>> CSVLoader::Load()
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file: " + path);
    }

    std::vector<std::vector<std::string>> result;
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> row = GLib::Utility::Split(line, ',');
        result.push_back(row);
    }

    return result;
}

