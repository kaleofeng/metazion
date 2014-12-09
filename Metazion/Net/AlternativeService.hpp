#ifndef _MZ_NET_ALTERNATIVESERVICE_HPP_
#define _MZ_NET_ALTERNATIVESERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollService.hpp"
#include "Metazion/Net/IocpService.hpp"
#include "Metazion/Net/SelectService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(NETWORK_USE_EPOLL_MODEL)
#   define SocketService EpollService
#endif // NETWORK_USE_EPOLL_MODEL

#if defined(NETWORK_USE_IOCP_MODEL)
#   define SocketService IocpService
#endif // NETWORK_USE_IOCP_MODEL

#if defined(NETWORK_USE_SELECT_MODEL)
#   define SocketService SelectService
#endif // NETWORK_USE_SELECT_MODEL

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ALTERNATIVESERVICE_HPP_
