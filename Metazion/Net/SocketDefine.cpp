#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

SockId_t CreateSockId(int transportType) {
    const int sockStream = TRANSPORT_TCP == transportType ? SOCK_STREAM : SOCK_DGRAM;

#if defined(MZ_PLATFORM_WINOWS)
    SockId_t sockId = ::WSASocket(AF_INET, sockStream, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    SockId_t sockId = ::socket(AF_INET, sockStream, IPPROTO_IP);
#endif // MZ_PLATFORM_LINUX

    if (INVALID_SOCKID != sockId) {
        SetNonBlock(sockId);
    }
    return sockId;
}

void DestroySockId(SockId_t sockId) {
#if defined(MZ_PLATFORM_WINOWS)
    ::closesocket(sockId);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    ::close(sockId);
#endif // MZ_PLATFORM_LINUX
}

bool SetNonBlock(SockId_t sockId) {
#if defined(MZ_PLATFORM_WINOWS)
    u_long ul = 1;
    return 0 == ::ioctlsocket(sockId, FIONBIO, &ul);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    int oldFlags = ::fcntl(sockId, F_GETFL);
    if (oldFlags == -1)  {
        oldFlags = 0;
    }
    return 0 == ::fcntl(sockId, F_SETFL, oldFlags | O_NONBLOCK);
#endif // MZ_PLATFORM_LINUX
}

bool IsWouldBlock() {
#if defined(MZ_PLATFORM_WINOWS)
    return WSAEWOULDBLOCK == ::WSAGetLastError();
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    return EINPROGRESS == errno;
#endif // MZ_PLATFORM_LINUX
}

int GetSockOpt(SockId_t sockId
    , int level
    , int optname
    , void* optval
    , SockLen_t* optlen) {
#if defined(MZ_PLATFORM_WINOWS)
    return ::getsockopt(sockId, level, optname, static_cast<char*>(optval), optlen);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    return ::getsockopt(sockId, level, optname, optval, optlen);
#endif // MZ_PLATFORM_LINUX
}

int SetSockOpt(SockId_t sockId
    , int level
    , int optname
    , const void* optval
    , SockLen_t optlen) {
#if defined(MZ_PLATFORM_WINOWS)
    return ::setsockopt(sockId, level, optname, static_cast<const char*>(optval), optlen);
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
    return ::setsockopt(sockId, level, optname, optval, optlen);
#endif // MZ_PLATFORM_LINUX

}

DECL_NAMESPACE_MZ_NET_END
