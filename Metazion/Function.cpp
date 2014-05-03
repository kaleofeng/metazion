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

DECL_NAMESPACE_MZ_END
