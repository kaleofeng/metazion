#ifndef _MZ_PLATFORM_HPP_
#define _MZ_PLATFORM_HPP_

// Current Platform
#if defined(_WIN32)
#   define MZ_PLATFORM_WINOWS
#elif defined(__linux__ )
#   define MZ_PLATFORM_LINUX
#else
#   define MZ_PLATFORM_OTHER
#endif // _WIN32

// Current Mode
#if defined(_DEBUG)
#   define MZ_MODE_DEBUG
#else
#   define MZ_MODE_RELEASE
#endif // _DEBUG

// Platform Dependent
#if defined(MZ_PLATFORM_WINOWS)
#   ifdef MZ_EXPORT_LIB
#       define MZEXPORT _declspec(dllexport)
#   else
#       define MZEXPORT _declspec(dllimport)
#   endif // MZ_EXPORT_LIB
#else
#   ifdef MZ_EXPORT_LIB
#       define MZEXPORT
#   else
#       define MZEXPORT
#   endif // MZ_EXPORT_LIB
#endif // MZ_PLATFORM_WINOWS

// Platform Headers
#if defined(MZ_PLATFORM_WINOWS)
#   define WIN32_LEAN_AND_MEAN 
#   include <Windows.h>
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
#   include <errno.h>
#   include <error.h>
#   include <unistd.h>
#   include <sys/time.h>
#   include <sys/types.h>
#endif // MZ_PLATFORM_LINUX

#include <algorithm>
#include <new>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#endif // _MZ_PLATFORM_HPP_
