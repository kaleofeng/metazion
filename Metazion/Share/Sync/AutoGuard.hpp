#ifndef MZ_SHARE_AUTOGUARD_HPP
#define MZ_SHARE_AUTOGUARD_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename T>
class AutoGuard {
    DISALLOW_COPY_AND_ASSIGN(AutoGuard)

    typedef T Lock_t;

public:
    AutoGuard(Lock_t& lock)
        : m_lock(lock) {
        m_lock.Lock();
    }

    ~AutoGuard() {
        m_lock.Unlock();
    }

private:
    Lock_t& m_lock;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_AUTOGUARD_HPP
