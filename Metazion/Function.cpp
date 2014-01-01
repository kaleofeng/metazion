#include "Metazion/Function.hpp"

DECL_NAMESPACE_MZ_BEGIN

int GetLastError() {
#if defined(MZ_PLATFORM_WINOWS)
    return ::GetLastError();
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    return errno;
#endif // MZ_PLATFORM_LINUX
}

void MilliSleep(int milliseconds) {
#if defined(MZ_PLATFORM_WINOWS)
    ::Sleep(milliseconds);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
    ::nanosleep(&ts, nullptr);
#endif // MZ_PLATFORM_LINUX
}

DECL_NAMESPACE_MZ_END
