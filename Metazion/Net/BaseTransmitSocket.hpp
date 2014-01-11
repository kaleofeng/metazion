#ifndef MZ_NET_BASETRANSMITSOCKET_HPP
#define MZ_NET_BASETRANSMITSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/IocpTransmitSocket.hpp"
#include "Metazion/Net/EpollTransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINOWS)
typedef IocpTransmitSocket BaseTransmitSocket;
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
typedef EpollTransmitSocket BaseTransmitSocket;
#endif // MZ_PLATFORM_LINUX

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_BASETRANSMITSOCKET_HPP
