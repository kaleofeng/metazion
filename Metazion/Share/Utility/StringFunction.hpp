#ifndef _MZ_SHARE_STRINGFUNCTION_HPP_
#define _MZ_SHARE_STRINGFUNCTION_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

int mzstrlen(const char* str);

char* mzstrcpy(char* dst, int size, const char* src);

char* mzstrcat(char* dst, int size, const char* src);

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_STRINGFUNCTION_HPP_
