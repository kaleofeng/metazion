#ifndef MZ_SHARE_SINGLETON_HPP
#define MZ_SHARE_SINGLETON_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename T>
class Singleton {
protected:
    Singleton() {}

    virtual ~Singleton() {}

private:
    Singleton(const Singleton& singleton);

    Singleton& operator =(const Singleton& singleton);

public:
    static T& Instance() { static T t; return t; }

    static T* Pointer() { return &Instance(); }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_SINGLETON_HPP