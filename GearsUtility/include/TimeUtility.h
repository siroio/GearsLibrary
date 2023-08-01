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
            using namespace std::chrono;
            auto now = system_clock::to_time_t(system_clock::now());
            tm localTime{};
            if (localtime_s(&localTime, &now) != 0) throw std::runtime_error("Failed to get local time.");
            return Time{ localTime.tm_hour, localTime.tm_min, localTime.tm_sec };
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
