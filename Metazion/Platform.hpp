#ifndef MZ_PLATFORM_HPP
#define MZ_PLATFORM_HPP

// Current platform
#if defined(_WIN32)
#   define MZ_PLATFORM_WINOWS
#else
#   define MZ_PLATFORM_LINUX
#endif // _WIN32

// Current mode
#if defined(_DEBUG)
#   define MZ_MODE_DEBUG
#else
#   define MZ_MODE_RELEASE
#endif // _DEBUG

// Platform dependent
#if defined(MZ_PLATFORM_WINOWS)
#   ifdef MZ_EXPORT_LIB
#       define _declspec(dllexport)
#   else
#       define _declspec(dllimport)
#   endif // MZ_EXPORT_LIB
#else
#   ifdef MZ_EXPORT_LIB
#       define MZEXPORT
#   else
#       define MZEXPORT
#   endif // MZ_EXPORT_LIB
#endif // MZ_PLATFORM_WINOWS

// Platform headers
#if defined(MZ_PLATFORM_WINOWS)
#   define WIN32_LEAN_AND_MEAN 
#   include <Windows.h>
// Check vc version.
#   if _MSC_VER >= 1600

#   else
#       define nullptr 0
#       define override
#       define final
#   endif // _MSC_VER >= 1600
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
#   include <unistd.h>
#   include <error.h>
#   include <errno.h> 
#   include <sys/types.h>
#   include <sys/time.h>
#endif // MZ_PLATFORM_LINUX

#include <new>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#endif // MZ_PLATFORM_HPP
