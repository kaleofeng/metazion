#ifndef _MZ_SHARE_THREADLOCALSTORAGE_HPP_
#define _MZ_SHARE_THREADLOCALSTORAGE_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class ThreadLocalStorage {
public:
    static int Alloc();

    static bool Free(int key);

    static void* GetValue(int key);

    static bool SetValue(int key, void* value);
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_THREADLOCALSTORAGE_HPP_
