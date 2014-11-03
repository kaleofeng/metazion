#ifndef _MZ_SHARE_STRINGHASH_HPP_
#define _MZ_SHARE_STRINGHASH_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

int32_t BKDRHash(const char* str) {
    uint32_t seed = 131;
    uint32_t hash = 0;

    while (*str != '\0') {
        hash = hash * seed + (*str++);
    }

    return hash & 0x7FFFFFFF;
}

int32_t APHash(const char* str) {
    uint32_t hash = 0;

    for (auto i = 0; *str!= '\0'; ++i) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return hash & 0x7FFFFFFF;
}

int32_t DJBHash(const char* str) {
    uint32_t hash = 5381;

    while (*str != '\0') {
        hash += (hash << 5) + (*str++);
    }

    return hash & 0x7FFFFFFF;
}

int32_t JSHash(const char* str) {
    uint32_t hash = 1315423911;

    while (*str != '\0') {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return hash & 0x7FFFFFFF;
}

int32_t RSHash(const char* str) {
    uint32_t b = 378551;
    uint32_t a = 63689;
    uint32_t hash = 0;

    while (*str != '\0') {
        hash = hash * a + (*str++);
        a *= b;
    }

    return hash & 0x7FFFFFFF;
}

int32_t SDBMHash(const char* str) {
    uint32_t hash = 0;

    while (*str != '\0') {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return hash & 0x7FFFFFFF;
}

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_STRINGHASH_HPP_
