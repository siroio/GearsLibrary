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
         * @brief CSV‚ğƒ[ƒh
         * @return 2ŸŒ³”z—ñ
         */
        strings Load() noexcept(false);
    };
}
