#ifndef _MZ_SHARE_MUTEXLOCK_LIN_HPP_
#define _MZ_SHARE_MUTEXLOCK_LIN_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include <pthread.h>

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MutexLock {
    DISALLOW_COPY_AND_ASSIGN(MutexLock)

public:
    MutexLock() {
        ::pthread_mutex_init(&m_mutex, NULL);
    }

    ~MutexLock() {
        ::pthread_mutex_destroy(&m_mutex);
    }

public:
    void Lock() {
        ::pthread_mutex_lock(&m_mutex);
    }

    void Unlock() {
        ::pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_LINUX

#endif // _MZ_SHARE_MUTEXLOCK_LIN_HPP_
