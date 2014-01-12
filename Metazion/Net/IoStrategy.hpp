#ifndef MZ_NET_IOSTRATEGY_HPP
#define MZ_NET_IOSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/IocpListenStrategy.hpp"
#include "Metazion/Net/IocpTransmitStrategy.hpp"
#include "Metazion/Net/EpollListenStrategy.hpp"
#include "Metazion/Net/EpollTransmitStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINOWS)
typedef IocpStrategy IoStrategy;
typedef IocpListenStrategy ListenStrategy;
typedef IocpTransmitStrategy TransmitStrategy;
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
typedef EpollStrategy IoStrategy;
typedef EpollListenStrategy ListenStrategy;
typedef EpollTransmitStrategy TransmitStrategy;
#endif // MZ_PLATFORM_LINUX

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_IOSTRATEGY_HPP
