#ifndef MZ_NET_BASELISTENSOCKET_HPP
#define MZ_NET_BASELISTENSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/IocpListenSocket.hpp"
#include "Metazion/Net/EpollListenSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINOWS)
typedef IocpListenSocket BaseListenSocket;
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
typedef EpollListenSocket BaseListenSocket;
#endif // MZ_PLATFORM_LINUX

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_BASELISTENSOCKET_HPP
