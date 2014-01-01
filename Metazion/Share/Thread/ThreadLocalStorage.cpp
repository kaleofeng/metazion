#include "Metazion/Share/Thread/ThreadLocalStorage.hpp"

#if defined(MZ_PLATFORM_LINUX)
#include <pthread.h>
#endif // MZ_PLATFORM_LINUX

DECL_NAMESPACE_MZ_SHARE_BEGIN

int ThreadLocalStorage::Alloc() {
#if defined(MZ_PLATFORM_WINOWS)
    const DWORD key = ::TlsAlloc();
    if (TLS_OUT_OF_INDEXES == key) {
        return -1;
    }
    return static_cast<int>(key);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    pthread_key_t key;
    if (0 != ::pthread_key_create(&key, NULL)) {
        return -1;
    }
    return static_cast<int>(key);
#endif // MZ_PLATFORM_LINUX
}

bool ThreadLocalStorage::Free(int key) {
#if defined(MZ_PLATFORM_WINOWS)
    const DWORD tKey = static_cast<DWORD>(key);
    return TRUE == ::TlsFree(tKey);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    const pthread_key_t tKey = static_cast<pthread_key_t>(key);
    return 0 == ::pthread_key_delete(tKey);
#endif // MZ_PLATFORM_LINUX
}

void* ThreadLocalStorage::GetValue(int key) {
#if defined(MZ_PLATFORM_WINOWS)
    const DWORD tKey = static_cast<DWORD>(key);
    return ::TlsGetValue(tKey);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    const pthread_key_t tKey = static_cast<pthread_key_t>(key);
    return ::pthread_getspecific(tKey);
#endif // MZ_PLATFORM_LINUX
}

bool ThreadLocalStorage::SetValue(int key, void* value) {
#if defined(MZ_PLATFORM_WINOWS)
    const DWORD tKey = static_cast<DWORD>(key);
    return TRUE == ::TlsSetValue(tKey, value);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    const pthread_key_t tKey = static_cast<pthread_key_t>(key);
    return 0 == ::pthread_setspecific(tKey, value);
#endif // MZ_PLATFORM_LINUX
}

DECL_NAMESPACE_MZ_SHARE_END
