#include "Metazion/Share/Utility/StringFunction.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

int mzstrlen(const char* str) {
    MZ_ASSERT_TRUE(!IsNull(str));

    auto count = 0;
    while (*str++ != '\0') {
        ++count;
    }

    return count;
}

char* mzstrcpy(char* dst, int size, const char* src) {
    MZ_ASSERT_TRUE(!IsNull(dst));
    MZ_ASSERT_TRUE(!IsNull(src));
    MZ_ASSERT_TRUE(size > 0);

    auto ptr = dst;
    auto count = size;
    while (count > 1 && (*ptr++ = *src++) != '\0') {
        --count;
    }

    dst[size - 1] = '\0';
    return dst;
}

char* mzstrcat(char* dst, int size, const char* src) {
    MZ_ASSERT_TRUE(!IsNull(dst));
    MZ_ASSERT_TRUE(!IsNull(src));
    MZ_ASSERT_TRUE(size > 0);

    auto ptr = dst;
    auto count = size;
    while (count > 1 && *ptr != '\0') {
        --count;
        ++ptr;
    }

    while (count > 1 && (*ptr++ = *src++) != '\0') {
        --count;
    }

    dst[size - 1] = '\0';
    return dst;
}

DECL_NAMESPACE_MZ_SHARE_END
