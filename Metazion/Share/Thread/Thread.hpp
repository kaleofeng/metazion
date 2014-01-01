#ifndef MZ_SHARE_THREAD_HPP
#define MZ_SHARE_THREAD_HPP

#include "Metazion/Share/ShareInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)
#include "Metazion/Share/Thread/Thread_Win.hpp"
#else
#include "Metazion/Share/Thread/Thread_Lin.hpp"
#endif // MZ_PLATFORM_WINOWS

#endif // MZ_SHARE_THREAD_HPP
