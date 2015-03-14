#ifndef _MZ_NET_SOCKETDEFINE_HPP_
#define _MZ_NET_SOCKETDEFINE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)
#include <winsock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif // MZ_PLATFORM_LINUX

#if defined(MZ_PLATFORM_MACOS)
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif // MZ_PLATFORM_MACOS

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINOWS)
typedef SOCKET SockId_t;
typedef int SockLen_t;
#define INVALID_SOCKID INVALID_SOCKET
#define SHUT_RD SD_SEND;
#define SHUT_WR SD_RECEIVE;
#define SHUT_RDWR SD_BOTH;
#define MSG_NOSIGNAL 0
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
typedef int SockId_t;
typedef socklen_t SockLen_t;
#define INVALID_SOCKID (-1)
#define SOCKET_ERROR (-1)
#endif // MZ_PLATFORM_LINUX

#if defined(MZ_PLATFORM_MACOS)
typedef int SockId_t;
typedef socklen_t SockLen_t;
#define INVALID_SOCKID (-1)
#define SOCKET_ERROR (-1)
#endif // MZ_PLATFORM_MACOS

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

int SAGetLastError();

bool IsInterrupted(int error);

bool IsWouldBlock(int error);

bool IsConnectWouldBlock(int error);

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKETDEFINE_HPP_
