#ifndef _MZ_NET_ALTERNATIVESERVICE_HPP_
#define _MZ_NET_ALTERNATIVESERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollService.hpp"
#include "Metazion/Net/IocpService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_LINUX)
#define SocketService EpollService
#endif // MZ_PLATFORM_LINUX

#if defined(MZ_PLATFORM_WINOWS)
#define SocketService IocpService
#endif // MZ_PLATFORM_WINOWS

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ALTERNATIVESERVICE_HPP_
