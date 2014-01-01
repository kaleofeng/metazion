#include "Metazion/Share/Time/Time.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

int32_t GetTickMillisecond() {
    int32_t millisecond = 0;

#if defined(MZ_PLATFORM_WINOWS)
    millisecond = ::GetTickCount();
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    timeval tv;
    ::gettimeofday(&tv, NULL);
    static int32_t last_sec = tv.tv_sec;
    tv.tv_sec -= last_sec;
    millisecond = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif // MZ_PLATFORM_LINUX

    return millisecond;
}

DECL_NAMESPACE_MZ_SHARE_END
