#ifndef IM_SHARE_MUTEXLOCK_HPP_
#define IM_SHARE_MUTEXLOCK_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)
#include "Metazion/Share/Sync/MutexLock_Win.hpp"
#else
#include "Metazion/Share/Sync/MutexLock_Lin.hpp"
#endif // MZ_PLATFORM_WINOWS

#endif // IM_SHARE_MUTEXLOCK_HPP_
