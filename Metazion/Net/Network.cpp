#include "Metazion/Net/Network.hpp"

#if defined(MZ_PLATFORM_WINOWS)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif // MZ_PLATFORM_WINOWS

DECL_NAMESPACE_MZ_NET_BEGIN

int Network::Startup() {
    int ret = 0;
#if defined(MZ_PLATFORM_WINOWS)
    WSADATA wsaData;
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif // MZ_PLATFORM_WINOWS
    return ret;
}

int Network::Cleanup() {
    int ret = 0;
#if defined(MZ_PLATFORM_WINOWS)
    ret = WSACleanup();
#endif // MZ_PLATFORM_WINOWS
    return ret;
}

DECL_NAMESPACE_MZ_NET_END
