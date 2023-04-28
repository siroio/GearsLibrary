#ifndef GEAR_CSV_LOADER_H
#define GEAR_CSV_LOADER_H

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

#endif // !GEAR_CSV_LOADER_H
