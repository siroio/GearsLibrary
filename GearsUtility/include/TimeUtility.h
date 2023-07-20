#pragma once
#include <chrono>
#include <sstream>

namespace Glib
{
    /**
     * @brief ŠÔŒnUtility
     */
    class TimeUtility
    {
    public:
        struct Time
        {
            int hours;
            int minutes;
            int seconds;

            Time() = default;
            Time(int hours, int minutes, int seconds)
                : hours(hours), minutes(minutes), seconds(seconds)
            {}
        };

        /**
         * @brief Œ»İ‚Ì‚ğ ŠÔ:•ª:•b‚Åæ“¾
         * @return Time
         */
        static inline Time CurrentTime()
        {
            auto now = std::chrono::system_clock::now();
            auto current_time = std::chrono::system_clock::to_time_t(now);
            std::tm* time_info = std::localtime(&current_time);

            Time current{
                time_info->tm_hour,
                time_info->tm_min,
                time_info->tm_sec
            };
            if (time_info != nullptr) delete time_info;
            return current;
        }

        /**
         * @brief Œ»İ‚Ì‚ğ•¶š—ñ‚Åæ“¾
         * @return [hh:mm:ss]
         */
        static inline std::string CurrentTimeStr()
        {
            const auto time = Glib::TimeUtility::CurrentTime();
            std::ostringstream ss{};
            ss << "[" << std::setfill('0') << std::setw(2) << time.hours
                << ":" << std::setfill('0') << std::setw(2) << time.minutes
                << ":" << std::setfill('0') << std::setw(2) << time.seconds << "]";
            return ss.str();
        }
    };
}
