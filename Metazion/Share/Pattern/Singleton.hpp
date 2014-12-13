#ifndef _MZ_SHARE_SINGLETON_HPP_
#define _MZ_SHARE_SINGLETON_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename T>
class Singleton {
    MZ_DISALLOW_COPY_AND_ASSIGN(Singleton);

protected:
    Singleton() {}

    virtual ~Singleton() {}

public:
    static T& Instance() { static T t; return t; }

    static T* Pointer() { return &Instance(); }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_SINGLETON_HPP_
