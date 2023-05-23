#ifndef GEARS_CSV_LOADER_H
#define GEARS_CSV_LOADER_H

#include <mutex>
#include <vector>
#include <string>
#include <string_view>

class CSVLoader
{
private:
    using strings = std::vector<std::vector<std::string>>;
    std::string path;
    std::mutex mutex;

public:
    CSVLoader(std::string_view path);
    ~CSVLoader();
    strings Load() noexcept(false);
};

#endif // !GEARS_CSV_LOADER_H
