#ifndef MZ_NET_BASESOCKETSERVER_HPP
#define MZ_NET_BASESOCKETSERVER_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/IocpSocketServer.hpp"
#include "Metazion/Net/EpollSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINOWS)
typedef IocpSocketServer BaseSocketServer;
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
typedef EpollSocketServer BaseSocketServer;
#endif // MZ_PLATFORM_LINUX

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_BASESOCKETSERVER_HPP
