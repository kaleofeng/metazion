#ifndef MZ_FUNCTION_HPP
#define MZ_FUNCTION_HPP

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

template<typename T> inline bool IsNull(const T& val) {
    return nullptr == val;
}

inline int AlignLength(int length, int align) {
    return (length + align - 1) & ~(align - 1);
}

inline bool IsLengthAlign(int length, int align) {
    return 0 == (length & (align - 1));
}

int GetLastError();

void MilliSleep(int milliseconds);

DECL_NAMESPACE_MZ_END
    
#endif // MZ_FUNCTION_HPP
