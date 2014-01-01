#include "Metazion/Net/EpollListenSocket.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include <Metazion/Share/Sync/AutoGuard.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

EpollListenSocket::EpollListenSocket() {}

EpollListenSocket::~EpollListenSocket() {}

void EpollListenSocket::Reset() {
    EpollSocket::Reset();
    m_localHost.Reset();
}

void EpollListenSocket::OnStarted() {
    EpollSocket::OnStarted();
}

bool EpollListenSocket::IsClosing() {
    bool ret = EpollSocket::IsClosing();
    if (ret) {
        return true;
    }

    return false;
}

bool EpollListenSocket::Listen(const char* ip, int port) {
    Host host;
    host.SetFamily(AF_INET);
    host.SetIp(ip);
    host.SetPort(port);

    const SockId_t sockId = CreateSockId(TRANSPORT_TCP);
    if (INVALID_SOCKID == sockId) {
        return false;
    }

    int optValue = 1;
    SockLen_t optLength = sizeof(optValue);
    int ret = SetSockOpt(sockId, SOL_SOCKET, SO_REUSEADDR, &optValue, optLength);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    SockAddr_t* sockAddr = host.SockAddr();
    SockLen_t sockAddrLen = host.SockAddrLen();
    ret = ::bind(sockId, sockAddr, sockAddrLen);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    ret = ::listen(sockId, 128);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    OpenSockId(sockId);
    m_localHost = host;
    return true;
}

void EpollListenSocket::Input() {
    Host peerHost;
    SockAddr_t* sockAddr = peerHost.SockAddr();
    SockLen_t sockAddrLen = peerHost.SockAddrLen();
    while (true) {
        SockId_t sockId = ::accept(m_sockId, sockAddr, &sockAddrLen);
        if (INVALID_SOCKID == sockId) {
            const int error = GetLastError();
            if (EINTR == error) {
                continue;
            }
            if (EAGAIN == error) {
               break;
            }

            switch(error) {
            case EMFILE:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case EPERM:
                ASSERT_TRUE(false);
                break;
            default: break;
            };

            ::printf("Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            Close();
            OnError(error);
            break;
        }

        SetNonBlock(sockId);

        if (!OnAccepted(sockId)) {
            DestroySockId(sockId);
        }
    }
}

void EpollListenSocket::Output() {}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
