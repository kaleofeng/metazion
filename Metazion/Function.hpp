#ifndef _MZ_FUNCTION_HPP_
#define _MZ_FUNCTION_HPP_

#include "Metazion/CoreInclude.hpp"

#include <cassert>

DECL_NAMESPACE_MZ_BEGIN

template<typename T>
inline void SafeDelete(T*& ptr) {
    enum { TYPE_MUST_BE_COMPLETE = sizeof(T) };
    delete ptr;
    ptr = nullptr; 
}

template<typename T>
inline void SafeDeleteArray(T*& ptr) {
    enum { TYPE_MUST_BE_COMPLETE = sizeof(T) };
    delete[] ptr;
    ptr = nullptr;
}

template<typename T>
inline bool IsNull(const T* const ptr) {
    return nullptr == ptr;
}

template<typename T>
inline bool IsNull(const T& val) {
    return nullptr == val;
}

inline int AlignLength(int length, int align) {
    return (length + align - 1) & ~(align - 1);
}

inline bool IsLengthAlign(int length, int align) {
    return (length & (align - 1)) == 0;
}

inline int MzGetLastError() {
#if defined(MZ_PLATFORM_WINOWS)
    return ::GetLastError();
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    return errno;
#endif // MZ_PLATFORM_LINUX
}

DECL_NAMESPACE_MZ_END
    
#endif // _MZ_FUNCTION_HPP_
