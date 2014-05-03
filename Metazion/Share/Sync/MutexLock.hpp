#ifndef IM_SHARE_MUTEXLOCK_HPP_
#define IM_SHARE_MUTEXLOCK_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <mutex>

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MutexLock {
    DISALLOW_COPY_AND_ASSIGN(MutexLock)

public:
    MutexLock() {}

    ~MutexLock() {}

public:
    void Lock() {
        m_mutex.lock();
    }

    bool TryLock() {
        return m_mutex.try_lock();
    }

    void Unlock() {
        m_mutex.unlock();
    }

private:
    std::mutex m_mutex;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // IM_SHARE_MUTEXLOCK_HPP_
