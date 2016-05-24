#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

SockId_t CreateSockId(int transportType) {
    const auto sockStream = transportType == TRANSPORT_TCP ? SOCK_STREAM : SOCK_DGRAM;

#if defined(MZ_PLATFORM_WINDOWS)
    auto sockId = WSASocket(AF_INET, sockStream, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
#endif

#if defined(MZ_PLATFORM_LINUX)
    auto sockId = socket(AF_INET, sockStream, IPPROTO_IP);
#endif

#if defined(MZ_PLATFORM_MACOS)
    auto sockId = socket(AF_INET, sockStream, IPPROTO_IP);
#endif

    if (sockId != INVALID_SOCKID) {
        SetNonBlock(sockId);
    }
    return sockId;
}

void DestroySockId(SockId_t sockId) {
#if defined(MZ_PLATFORM_WINDOWS)
    closesocket(sockId);
#endif

#if defined(MZ_PLATFORM_LINUX)
    close(sockId);
#endif

#if defined(MZ_PLATFORM_MACOS)
    close(sockId);
#endif
}

bool SetNonBlock(SockId_t sockId) {
#if defined(MZ_PLATFORM_WINDOWS)
    u_long ul = 1;
    return 0 == ioctlsocket(sockId, FIONBIO, &ul);
#endif

#if defined(MZ_PLATFORM_LINUX)
    auto oldFlags = fcntl(sockId, F_GETFL);
    if (oldFlags == -1)  {
        oldFlags = 0;
    }
    return 0 == fcntl(sockId, F_SETFL, oldFlags | O_NONBLOCK);
#endif

#if defined(MZ_PLATFORM_MACOS)
    auto oldFlags = fcntl(sockId, F_GETFL);
    if (oldFlags == -1) {
        oldFlags = 0;
    }
    return 0 == fcntl(sockId, F_SETFL, oldFlags | O_NONBLOCK);
#endif
}

int GetSockOpt(SockId_t sockId
    , int level
    , int optname
    , void* optval
    , SockLen_t* optlen) {
#if defined(MZ_PLATFORM_WINDOWS)
    return getsockopt(sockId, level, optname, static_cast<char*>(optval), optlen);
#endif

#if defined(MZ_PLATFORM_LINUX)
    return getsockopt(sockId, level, optname, optval, optlen);
#endif

#if defined(MZ_PLATFORM_MACOS)
    return getsockopt(sockId, level, optname, optval, optlen);
#endif
}

int SetSockOpt(SockId_t sockId
    , int level
    , int optname
    , const void* optval
    , SockLen_t optlen) {
#if defined(MZ_PLATFORM_WINDOWS)
    return setsockopt(sockId, level, optname, static_cast<const char*>(optval), optlen);
#endif

#if defined(MZ_PLATFORM_LINUX)
    return setsockopt(sockId, level, optname, optval, optlen);
#endif

#if defined(MZ_PLATFORM_MACOS)
    return setsockopt(sockId, level, optname, optval, optlen);
#endif
}

int SAGetLastError() {
#if defined(MZ_PLATFORM_WINDOWS)
    return WSAGetLastError();
#endif

#if defined(MZ_PLATFORM_LINUX)
     return errno;
#endif

#if defined(MZ_PLATFORM_MACOS)
     return errno;
#endif
}

bool IsInterrupted(int error) {
#if defined(MZ_PLATFORM_WINDOWS)
    return false;
#endif

#if defined(MZ_PLATFORM_LINUX)
    return error == EINTR;
#endif

#if defined(MZ_PLATFORM_MACOS)
    return error == EINTR;
#endif
}

bool IsWouldBlock(int error) {
#if defined(MZ_PLATFORM_WINDOWS)
    return error == WSAEWOULDBLOCK;
#endif

#if defined(MZ_PLATFORM_LINUX)
    return error == EAGAIN || error == EWOULDBLOCK;
#endif

#if defined(MZ_PLATFORM_MACOS)
    return error == EAGAIN || error == EWOULDBLOCK;
#endif
}

bool IsConnectWouldBlock(int error) {
#if defined(MZ_PLATFORM_WINDOWS)
    return error == WSAEWOULDBLOCK;
#endif

#if defined(MZ_PLATFORM_LINUX)
    return error == EINPROGRESS;
#endif

#if defined(MZ_PLATFORM_MACOS)
    return error == EINPROGRESS;
#endif
}

DECL_NAMESPACE_MZ_NET_END
