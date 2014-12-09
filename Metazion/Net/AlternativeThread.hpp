#ifndef _MZ_NET_ALTERNATIVETHREAD_HPP_
#define _MZ_NET_ALTERNATIVETHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollIoThread.hpp"
#include "Metazion/Net/IocpIoThread.hpp"
#include "Metazion/Net/SelectIoThread.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(NETWORK_USE_EPOLL_MODEL)
#   define IoThread EpollIoThread
#endif // NETWORK_USE_EPOLL_MODEL

#if defined(NETWORK_USE_IOCP_MODEL)
#   define IoThread IocpIoThread
#endif // NETWORK_USE_IOCP_MODEL

#if defined(NETWORK_USE_SELECT_MODEL)
#   define IoThread SelectIoThread
#endif // NETWORK_USE_SELECT_MODEL

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ALTERNATIVETHREAD_HPP_
