#include "Metazion/Net/Network.hpp"

#if defined(MZ_PLATFORM_WINDOWS)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

DECL_NAMESPACE_MZ_NET_BEGIN

int Network::Startup() {
    int ret = 0;
#if defined(MZ_PLATFORM_WINDOWS)
    WSADATA wsaData;
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    return ret;
}

int Network::Cleanup() {
    int ret = 0;
#if defined(MZ_PLATFORM_WINDOWS)
    ret = WSACleanup();
#endif
    return ret;
}

DECL_NAMESPACE_MZ_NET_END
