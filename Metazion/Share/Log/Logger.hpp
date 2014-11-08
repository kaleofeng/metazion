#ifndef _MZ_SHARE_LOGGER_HPP_
#define _MZ_SHARE_LOGGER_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Log/LogDefine.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename T, typename ...Args>
void Log(T type, const char* format, Args... args) {
    ::printf(format, args...);
}

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_LOGGER_HPP_
