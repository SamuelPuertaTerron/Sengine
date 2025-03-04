#include "Globals.h"
#include "Time.h"

namespace Sengine
{
    //TODO: Make this support linux with the localtime_r function
    std::string Time::GetCurrentTimeAsString()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        std::stringstream ss;
#ifdef SE_PLATFORM_WINDOWS
        errno_t t = localtime_s(&tm, &time);
        if (t != 0)
        {
            return "0:0:0";
        }
#endif
#ifdef SE_PLATFORM_LINUX
        if (localtime_r(&time, &tm) != nullptr_t)
        {
            return "0:0:0";
        }
#endif
        ss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
        return ss.str();
    }

    std::string Time::GetCurrentDateAsString()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        std::stringstream ss;
#ifdef SE_PLATFORM_WINDOWS
        errno_t t = localtime_s(&tm, &time);
        if (t != 0)
        {
            return "0:0:0";
        }
#endif
#ifdef SE_PLATFORM_LINUX
        if (localtime_r(&time, &tm) != nullptr_t)
        {
            return "0:0:0";
        }
#endif
        ss << std::put_time(&tm, "%d-%m-%Y");
        return ss.str();
    }

    TimePoint Time::GetTime()
    {
        return HighResClock::now();
    }
}