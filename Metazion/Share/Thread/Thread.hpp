#ifndef _MZ_SHARE_THREAD_HPP_
#define _MZ_SHARE_THREAD_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)
#include "Metazion/Share/Thread/Thread_Win.hpp"
#else
#include "Metazion/Share/Thread/Thread_Lin.hpp"
#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_SHARE_THREAD_HPP_
