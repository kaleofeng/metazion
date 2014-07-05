#ifndef _MZ_NET_ALTERNATIVETHREAD_HPP_
#define _MZ_NET_ALTERNATIVETHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollIoThread.hpp"
#include "Metazion/Net/IocpIoThread.hpp"
#include "Metazion/Net/MaintenanceThread.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_LINUX)
#define IoThread EpollIoThread
#endif // MZ_PLATFORM_LINUX

#if defined(MZ_PLATFORM_WINOWS)
#define IoThread IocpIoThread
#endif // MZ_PLATFORM_WINOWS

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ALTERNATIVETHREAD_HPP_
