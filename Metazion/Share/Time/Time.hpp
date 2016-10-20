#ifndef _MZ_SHARE_TIME_HPP_
#define _MZ_SHARE_TIME_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

struct Calendar {

    Calendar() {}

    Calendar(int year, int month, int day, int hour, int minute, int second)
        : m_year(year)
        , m_month(month)
        , m_day(day)
        , m_hour(hour)
        , m_minute(minute)
        , m_second(second) {}

    int m_year{ 0 };
    int m_month{ 0 };
    int m_day{ 0 };
    int m_hour{ 0 };
    int m_minute{ 0 };
    int m_second{ 0 };
    int m_dayOfWeek{ 0 };
    int m_dayOfYear{ 0 };
};

int64_t GetNowMillisecond();

int64_t GetNowMicrosecond();

Calendar TimeToLocalCalendar(int64_t ms);

int64_t CalendarToLocalTime(const Calendar& calendar);

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_TIME_HPP_
