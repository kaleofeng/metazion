#include "Metazion/Share/Time/Time.hpp"

#include <ctime>
#include <chrono>

using namespace std::chrono;

DECL_NAMESPACE_MZ_SHARE_BEGIN

int64_t GetNowMillisecond() {
    const auto timePoint = system_clock::now();
    const auto timeDuration = timePoint.time_since_epoch();
    return duration_cast<milliseconds>(timeDuration).count();
}

int64_t GetNowMicrosecond() {
    const auto timePoint = system_clock::now();
    const auto timeDuration = timePoint.time_since_epoch();
    return duration_cast<microseconds>(timeDuration).count();
}

Calendar TimeToLocalCalendar(int64_t ms) {
    const auto td = std::chrono::milliseconds(ms);
    const auto tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(td);
    const auto tt = std::chrono::system_clock::to_time_t(tp);
    const auto lt = std::localtime(&tt);
    Calendar calendar;
    calendar.m_year = lt->tm_year + 1900;
    calendar.m_month = lt->tm_mon + 1;
    calendar.m_day = lt->tm_mday;
    calendar.m_hour = lt->tm_hour;
    calendar.m_minute = lt->tm_min;
    calendar.m_second = lt->tm_sec;
    calendar.m_dayOfWeek = lt->tm_wday;
    calendar.m_dayOfYear = lt->tm_yday;
    return calendar;
}

int64_t CalendarToLocalTime(const Calendar& calendar) {
    tm lt;
    lt.tm_year = calendar.m_year - 1900;
    lt.tm_mon = calendar.m_month - 1;
    lt.tm_mday = calendar.m_day;
    lt.tm_hour = calendar.m_hour;
    lt.tm_min = calendar.m_minute;
    lt.tm_sec = calendar.m_second;
    const auto tt = std::mktime(&lt);
    const auto tp = system_clock::from_time_t(tt);
    const auto td = tp.time_since_epoch();
    const auto ms = duration_cast<milliseconds>(td).count();
    return ms;
}

DECL_NAMESPACE_MZ_SHARE_END
