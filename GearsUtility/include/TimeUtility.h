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
            //using namespace std::chrono;
            //zoned_time now{ current_zone(), system_clock::now() };
            //auto localTime = now.get_local_time();
            //auto time_point = localTime.time_since_epoch();
            //auto h = std::chrono::duration_cast<hours>(time_point);
            //time_point -= h;
            //auto m = std::chrono::duration_cast<minutes>(time_point);
            //time_point -= m;
            //auto s = std::chrono::duration_cast<seconds>(time_point);

            return Time{ 0,0,0 };
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
