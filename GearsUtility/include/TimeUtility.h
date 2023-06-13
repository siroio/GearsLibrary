#ifndef GEARS_TIME_UTILITY_H
#define GEARS_TIME_UTILITY_H

#include <chrono>
#include <sstream>

namespace GLib
{
    struct Time
    {
        int hours;
        int minutes;
        int seconds;
    };

    /**
     * @brief ŠÔŒnUtility
     */
    class TimeUtility
    {
    public:
        /**
         * @brief Œ»İ‚Ì‚ğ ŠÔ:•ª:•b‚Åæ“¾
         * @return Time
         */
        static inline Time CurrentTime()
        {
            auto currentTime = std::chrono::system_clock::now();
            std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
            std::tm localTime;
#ifdef _WIN32
            localtime_s(&localTime, &currentTime_t);
#else
            localtime_r(&currentTime_t, &localTime);
#endif
            Time result
            {
                localTime.tm_hour,
                localTime.tm_min,
                localTime.tm_sec
            };

            return result;
        }

        /**
         * @brief Œ»İ‚Ì‚ğ•¶š—ñ‚Åæ“¾
         * @return [hh:mm:ss]
         */
        static inline std::string CurrentTimeStr()
        {
            const auto time = GLib::TimeUtility::CurrentTime();
            std::ostringstream ss{};
            ss << "[" << std::setfill('0') << std::setw(2) << time.hours
                << ":" << std::setfill('0') << std::setw(2) << time.minutes
                << ":" << std::setfill('0') << std::setw(2) << time.seconds << "]";
            return ss.str();
        }
    };
}

#endif // !GEARS_TIME_UTILITY_H
