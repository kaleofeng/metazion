#ifndef _MZ_SHARE_HASHER_HPP_
#define _MZ_SHARE_HASHER_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Algorithm/StringHash.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename T>
struct IntegerHasher {
    int32_t operator ()(const T& value) const {
        const auto magicNumber = 13;
        auto hash = static_cast<uint32_t>(value);
        hash *= magicNumber;
        return hash & 0x7fffffff;
    }
};

template<typename T>
struct PointerHasher {
    int32_t operator ()(const T& value) const {
        const auto magicNumber = 13;
        auto ptr = reinterpret_cast<uintptr_t>(value);
        auto hash = static_cast<uint32_t>(ptr);
        hash >>= 2;
        hash *= magicNumber;
        return hash & 0x7fffffff;
    }
};

template<typename T>
struct StringHasher {
    int32_t operator ()(const T& value) const {
        const auto str = static_cast<const char*>(value);
        const auto hash = BKDRHash(str);
        return hash & 0x7fffffff;
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_HASHER_HPP_
 