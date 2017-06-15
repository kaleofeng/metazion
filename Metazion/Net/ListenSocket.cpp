#include "Metazion/Net/ListenSocket.hpp"

#include <Metazion/Share/Log/Logger.hpp>
#include <Metazion/Share/Time/Time.hpp>

#include "Metazion/Net/NetworkService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

ListenSocket::ListenSocket()
    : m_listenStrategy(*this) {}

ListenSocket::~ListenSocket() {}

void ListenSocket::Reset() {
    Socket::Reset();

    m_listenStrategy.Reset();
    m_localHost.Reset();

    m_watchedTime = 0;
    m_unwatchedTime = 0;
    m_firstAcceptTime = 0;
    m_lastAcceptTime = 0;
    m_acceptedNumber = 0;
}

void ListenSocket::Prepare() {
    Socket::Prepare();

    m_listenStrategy.Prepare();
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

bool ListenSocket::KeepEnough() const {
    return false;
}

void ListenSocket::OnAttached() {}

void ListenSocket::OnDetached() {}

void ListenSocket::OnStart() {
    m_listenStrategy.Start();
    OnWatched();
}

void ListenSocket::OnStop() {
    OnUnwatched();
}

void ListenSocket::OnError(int error) {}

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
    ret = bind(sockId, sockAddr, sockAddrLen);
    if (ret == SOCKET_ERROR) {
        DestroySockId(sockId);
        return false;
    }

    ret = listen(sockId, backlog);
    if (ret == SOCKET_ERROR) {
        DestroySockId(sockId);
        return false;
    }

    AttachSockId(sockId);
    return true;
}

void ListenSocket::OnWatched() {
    m_watchedTime = m_now;
}

void ListenSocket::OnUnwatched() {
    m_unwatchedTime = m_now;
}

bool ListenSocket::OnAccepted(const SockId_t& sockId, const Host& host) {
    const auto now = m_now;
    if (m_firstAcceptTime == 0) {
        m_firstAcceptTime = now;
    }

    m_lastAcceptTime = now;
    ++m_acceptedNumber;
    return true;
}

DECL_NAMESPACE_MZ_NET_END
