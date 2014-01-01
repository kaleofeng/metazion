#ifndef MZ_NET_SOCKETDEFINE_HPP
#define MZ_NET_SOCKETDEFINE_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)
#include <winsock2.h>
#include <mswsock.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#endif // MZ_PLATFORM_LINUX

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINOWS)
typedef SOCKET SockId_t;
typedef int SockLen_t;
#define INVALID_SOCKID INVALID_SOCKET
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
typedef int SockId_t;
typedef socklen_t SockLen_t;
#define INVALID_SOCKID (-1)
#define SOCKET_ERROR (-1)
#endif // MZ_PLATFORM_LINUX

enum TansportType {
    TRANSPORT_TCP = 1,
    TRANSPORT_UDP,
};

enum SocketType {
    SOCKET_TCP_LISTEN = 1,
    SOCKET_TCP_SERVER,
    SOCKET_TCP_CLIENT,
};

typedef struct sockaddr SockAddr_t;
typedef struct sockaddr_in SockAddrIn_t;

SockId_t CreateSockId(int transportType);

void DestroySockId(SockId_t sockId);

bool SetNonBlock(SockId_t sockId);

bool IsWouldBlock();

int GetSockOpt(SockId_t sockId
    , int level
    , int optname
    , void* optval
    , SockLen_t* optlen);

int SetSockOpt(SockId_t sockId
    , int level
    , int optname
    , const void* optval
    , SockLen_t optlen);

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_SOCKETDEFINE_HPP
