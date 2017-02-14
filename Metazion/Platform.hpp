#ifndef _MZ_PLATFORM_HPP_
#define _MZ_PLATFORM_HPP_

// Current Platform
#if defined(_WIN32)
#   define MZ_PLATFORM_WINDOWS
#elif defined(__linux__ )
#   define MZ_PLATFORM_LINUX
#elif defined(__APPLE__)
#   define MZ_PLATFORM_MACOS
#else
#   define MZ_PLATFORM_OTHER
#endif

// Current Mode
#if defined(_DEBUG)
#   define MZ_MODE_DEBUG
#else
#   define MZ_MODE_RELEASE
#endif

// Platform Dependent
#if defined(MZ_PLATFORM_WINDOWS)
#   if defined(MZ_EXPORT_LIB)
#       define MZEXPORT _declspec(dllexport)
#   else
#       define MZEXPORT _declspec(dllimport)
#   endif
#else
#   if defined(MZ_EXPORT_LIB)
#       define MZEXPORT
#   else
#       define MZEXPORT
#   endif
#endif

// Platform Headers
#if defined(MZ_PLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN 
#   include <Windows.h>
#endif

#if defined(MZ_PLATFORM_LINUX)
#   include <errno.h>
#   include <error.h>
#   include <fcntl.h>
#   include <poll.h>
#   include <signal.h>
#   include <sys/syscall.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
#endif

#if defined(MZ_PLATFORM_MACOS)
#   include <errno.h>
#   include <fcntl.h>
#   include <poll.h>
#   include <signal.h>
#   include <sys/syscall.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
#endif

#include <algorithm>
#include <new>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#endif // _MZ_PLATFORM_HPP_
