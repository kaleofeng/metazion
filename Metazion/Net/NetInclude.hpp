#ifndef _MZ_NET_INCLUDE_HPP_
#define _MZ_NET_INCLUDE_HPP_

#include "Metazion/Overall.hpp"

#define DECL_NAMESPACE_MZ_NET_BEGIN     namespace mz { namespace net {
#define DECL_NAMESPACE_MZ_NET_END       } }
#define USING_NAMESPACE_MZ_NET          using namespace mz::net;
#define USING_NAMESPACE_NET             using namespace net;
#define NS_MZ_NET                       mz::net
#define NS_NET                          net

#if defined(MZ_PLATFORM_LINUX1)
#   define NETWORK_USE_EPOLL_MODEL
#elif defined(MZ_PLATFORM_WINDOWS1)
#   define NETWORK_USE_IOCP_MODEL
#else
#   define NETWORK_USE_SELECT_MODEL
#endif

#endif // _MZ_NET_INCLUDE_HPP_
