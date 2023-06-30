#pragma once
#include <mutex>
#include <vector>
#include <string>
#include <string_view>

namespace Glib
{
    class CSVLoader
    {
    private:
        using strings = std::vector<std::vector<std::string>>;
        std::string path;
        std::mutex mutex;

    public:
        CSVLoader(std::string_view path);
        ~CSVLoader();
        /**
         * @brief CSV�����[�h
         * @return 2�����z��
         */
        strings Load() noexcept(false);
    };
}
