#ifndef MZ_SHARE_MUTEXLOCK_WIN_HPP
#define MZ_SHARE_MUTEXLOCK_WIN_HPP

#include "Metazion/Share/ShareInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MutexLock {
    DISALLOW_COPY_AND_ASSIGN(MutexLock)

public:
    MutexLock() {
        ::InitializeCriticalSection (&m_cs);
    }

    ~MutexLock() {
        ::DeleteCriticalSection (&m_cs);
    }

    void Lock() {
        ::EnterCriticalSection (&m_cs);
    }

    void Unlock() {
        ::LeaveCriticalSection (&m_cs);
    }

private:
    CRITICAL_SECTION m_cs;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_WINOWS

#endif // MZ_SHARE_MUTEXLOCK_WIN_HPP
