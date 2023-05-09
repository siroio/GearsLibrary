#ifndef GEARS_CSV_LOADER_H
#define GEARS_CSV_LOADER_H

#include <mutex>
#include <vector>

class CSVLoader
{
private:
    std::string path;
    std::mutex mutex;

public:
    CSVLoader(const char* path);
    ~CSVLoader();
    std::vector<std::vector<std::string>> Load();
};

#endif // !GEARS_CSV_LOADER_H
