#ifndef GEARS_TIME_UTILITY_H
#define GEARS_TIME_UTILITY_H

#include <chrono>
#include <sstream>

namespace GLib
{
    /**
     * @brief 時間系Utility
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

            bool operator == (const Time& other) const = default;
        };

        /**
         * @brief 現在の時刻を 時間:分:秒で取得
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
            return Time{
                localTime.tm_hour,
                localTime.tm_min,
                localTime.tm_sec
            };
        }

        /**
         * @brief 現在の時刻を文字列で取得
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
