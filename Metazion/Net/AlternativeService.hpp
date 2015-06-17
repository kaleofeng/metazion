#ifndef _MZ_NET_ALTERNATIVESERVICE_HPP_
#define _MZ_NET_ALTERNATIVESERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollService.hpp"
#include "Metazion/Net/IocpService.hpp"
#include "Metazion/Net/SelectService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(NETWORK_USE_EPOLL_MODEL)
#   define SocketService EpollService
#endif

#if defined(NETWORK_USE_IOCP_MODEL)
#   define SocketService IocpService
#endif

#if defined(NETWORK_USE_SELECT_MODEL)
#   define SocketService SelectService
#endif

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ALTERNATIVESERVICE_HPP_
