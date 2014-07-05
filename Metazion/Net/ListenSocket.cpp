#include "Metazion/Net/ListenSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

ListenSocket::ListenSocket()
    : m_listenStrategy(*this) {}

ListenSocket::~ListenSocket() {}

IoStrategy& ListenSocket::GetIoStrategy() {
    return m_listenStrategy;
}

void ListenSocket::Reset() {
    Socket::Reset();
    m_localHost.Reset();
    m_listenStrategy.Reset();
}

void ListenSocket::OnStarted() {
    Socket::OnStarted();
    m_listenStrategy.OnStarted();
}

bool ListenSocket::IsAlive() const {
    auto ret = Socket::IsActive();
    if (ret) {
        return true;
    }

    ret = m_listenStrategy.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

bool ListenSocket::OnAccepted(const SockId_t& sockId) {
    return true;
}

void ListenSocket::SetLocalHost(const char* ip, int port) {
    m_localHost.SetFamily(AF_INET);
    m_localHost.SetIp(ip);
    m_localHost.SetPort(port);
}

bool ListenSocket::Listen(int backlog) {
    const auto sockId = CreateSockId(TRANSPORT_TCP);
    if (INVALID_SOCKID == sockId) {
        return false;
    }

    int optValue = 1;
    auto optLength = static_cast<SockLen_t>(sizeof(optValue));
    auto ret = SetSockOpt(sockId, SOL_SOCKET, SO_REUSEADDR, &optValue, optLength);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    auto sockAddr = m_localHost.SockAddr();
    auto sockAddrLen = m_localHost.SockAddrLen();
    ret = ::bind(sockId, sockAddr, sockAddrLen);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    ret = ::listen(sockId, backlog);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    AttachSockId(sockId);
    return true;
}

DECL_NAMESPACE_MZ_NET_END
