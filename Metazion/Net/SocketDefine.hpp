#ifndef _MZ_NET_SOCKETDEFINE_HPP_
#define _MZ_NET_SOCKETDEFINE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINDOWS)
#include <winsock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif

#if defined(MZ_PLATFORM_LINUX)
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif

#if defined(MZ_PLATFORM_MACOS)
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINDOWS)
    typedef SOCKET SockId_t;
    typedef int SockLen_t;
#   define INVALID_SOCKID INVALID_SOCKET
#   define SHUT_RD SD_SEND
#   define SHUT_WR SD_RECEIVE
#   define SHUT_RDWR SD_BOTH

#   define IOV_TYPE WSABUF
#   define IOV_PTR_FIELD buf
#   define IOV_LEN_FIELD len
#   define IOV_LEN_TYPE ULONG

#   define SINGLE_BUFFER_LENGTH 256
#   define SINGLE_BUFFER_SIZE 1024

#   define NUMBER_SEND_IOV 16
#   define NUMBER_RECV_IOV 16
#endif

#if defined(MZ_PLATFORM_LINUX)
    typedef int SockId_t;
    typedef socklen_t SockLen_t;
#   define INVALID_SOCKID (-1)
#   define SOCKET_ERROR (-1)

#   define IOV_TYPE struct iovec
#   define IOV_PTR_FIELD iov_base
#   define IOV_LEN_FIELD iov_len
#   define IOV_LEN_TYPE size_t

#   define SINGLE_BUFFER_LENGTH 256
#   define SINGLE_BUFFER_SIZE 1024

#   define NUMBER_SEND_IOV 16
#   define NUMBER_RECV_IOV 16
#endif

#if defined(MZ_PLATFORM_MACOS)
    typedef int SockId_t;
    typedef socklen_t SockLen_t;
#   define INVALID_SOCKID (-1)
#   define SOCKET_ERROR (-1)

#   define IOV_TYPE struct iovec
#   define IOV_PTR_FIELD iov_base
#   define IOV_LEN_FIELD iov_len
#   define IOV_LEN_TYPE size_t

#   define SINGLE_BUFFER_LENGTH 256
#   define SINGLE_BUFFER_SIZE 1024

#   define NUMBER_SEND_IOV 1
#   define NUMBER_RECV_IOV 1
#endif

#define LENGTH_SEND_MAX 4096
#define LENGTH_RECV_MAX 4096

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

void ShutdownSockId(SockId_t sockId, int how);

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
