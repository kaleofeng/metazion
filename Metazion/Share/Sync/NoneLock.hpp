#ifndef MZ_SHARE_NONELOCK_HPP
#define MZ_SHARE_NONELOCK_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class NoneLock {
    DISALLOW_COPY_AND_ASSIGN(NoneLock)

public:
    NoneLock() {}

    ~NoneLock() {}

public:
    void Lock() {}

    void Unlock() {}
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_NONELOCK_HPP
