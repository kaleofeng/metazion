#ifndef _MZ_SHARE_NONELOCK_HPP_
#define _MZ_SHARE_NONELOCK_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class NoneLock {
    DISALLOW_COPY_AND_ASSIGN(NoneLock)

public:
    NoneLock() {}

    ~NoneLock() {}

public:
    void lock() {}

    bool try_lock() { return true; }

    void unlock() {}
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_NONELOCK_HPP_
