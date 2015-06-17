#ifndef _MZ_NET_ALTERNATIVESTRATEGY_HPP_
#define _MZ_NET_ALTERNATIVESTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollListenStrategy.hpp"
#include "Metazion/Net/EpollTransmitStrategy.hpp"
#include "Metazion/Net/IocpListenStrategy.hpp"
#include "Metazion/Net/IocpTransmitStrategy.hpp"
#include "Metazion/Net/SelectListenStrategy.hpp"
#include "Metazion/Net/SelectTransmitStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(NETWORK_USE_EPOLL_MODEL)
#   define IoStrategy EpollStrategy
#   define ListenStrategy EpollListenStrategy
#   define TransmitStrategy EpollTransmitStrategy
#endif

#if defined(NETWORK_USE_IOCP_MODEL)
#   define IoStrategy IocpStrategy
#   define ListenStrategy IocpListenStrategy
#   define TransmitStrategy IocpTransmitStrategy
#endif

#if defined(NETWORK_USE_SELECT_MODEL)
#   define IoStrategy SelectStrategy
#   define ListenStrategy SelectListenStrategy
#   define TransmitStrategy SelectTransmitStrategy
#endif

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ALTERNATIVESTRATEGY_HPP_
