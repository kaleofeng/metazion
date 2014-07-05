#ifndef _MZ_NET_ALTERNATIVESTRATEGY_HPP_
#define _MZ_NET_ALTERNATIVESTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollListenStrategy.hpp"
#include "Metazion/Net/EpollTransmitStrategy.hpp"
#include "Metazion/Net/IocpListenStrategy.hpp"
#include "Metazion/Net/IocpTransmitStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_LINUX)
#define IoStrategy EpollStrategy
#define ListenStrategy EpollListenStrategy
#define TransmitStrategy EpollTransmitStrategy
#endif // MZ_PLATFORM_LINUX

#if defined(MZ_PLATFORM_WINOWS)
#define IoStrategy IocpStrategy
#define ListenStrategy IocpListenStrategy
#define TransmitStrategy IocpTransmitStrategy
#endif // MZ_PLATFORM_WINOWS

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ALTERNATIVESTRATEGY_HPP_
