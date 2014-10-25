#include "Metazion/Net/ListenSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

ListenSocket::ListenSocket()
    : m_listenStrategy(*this) {}

ListenSocket::~ListenSocket() {}

void ListenSocket::Reset() {
    Socket::Reset();
    m_localHost.Reset();
    m_listenStrategy.Reset();
}

void ListenSocket::Prepare() {
    Socket::Prepare();
    m_listenStrategy.Prepare();
}

void ListenSocket::Tick(int interval) {
    Socket::Tick(interval);
    m_listenStrategy.Tick(interval);
}

IoStrategy& ListenSocket::GetIoStrategy() {
    return m_listenStrategy;
}

bool ListenSocket::IsAlive() const {
    auto ret = IsValid();
    if (ret) {
        return true;
    }

    ret = m_listenStrategy.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

void ListenSocket::OnAttached() {}

void ListenSocket::OnDetached() {}

void ListenSocket::OnStart() {
    m_listenStrategy.Start();
    OnWatched();
}

void ListenSocket::OnClose() {
    OnUnwatched();
}

bool ListenSocket::OnError(int error) {
    return true;
}

void ListenSocket::SetLocalHost(const Host& host) {
    m_localHost = host;
}

bool ListenSocket::Listen(int backlog) {
    const auto sockId = CreateSockId(TRANSPORT_TCP);
    if (sockId == INVALID_SOCKID) {
        return false;
    }

    int optValue = 1;
    auto optLength = static_cast<SockLen_t>(sizeof(optValue));
    auto ret = SetSockOpt(sockId, SOL_SOCKET, SO_REUSEADDR, &optValue, optLength);
    if (ret == SOCKET_ERROR) {
        DestroySockId(sockId);
        return false;
    }

    auto sockAddr = m_localHost.SockAddr();
    auto sockAddrLen = m_localHost.SockAddrLen();
    ret = ::bind(sockId, sockAddr, sockAddrLen);
    if (ret == SOCKET_ERROR) {
        DestroySockId(sockId);
        return false;
    }

    ret = ::listen(sockId, backlog);
    if (ret == SOCKET_ERROR) {
        DestroySockId(sockId);
        return false;
    }

    AttachSockId(sockId);
    return true;
}

DECL_NAMESPACE_MZ_NET_END
