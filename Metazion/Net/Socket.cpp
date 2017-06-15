#include "Metazion/Net/Socket.hpp"

#include "Metazion/Share/Time/Time.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

Socket::Socket() {}

Socket::~Socket() {}

void Socket::Reset() {
    m_reference = 0;
    m_sockId = INVALID_SOCKID;
    m_index = -1;
    m_networkService = nullptr;
    m_keepInterval = 0;
    m_destroyCallback = [](Socket* socket) { delete socket; };
    m_now = NS_MZ_SHARE::GetNowMillisecond();
    m_working = false;
    m_gonnaClose = false;
    m_delayTime = 0;
    m_wannaClose = false;
}

void Socket::Prepare() {
    m_now = NS_MZ_SHARE::GetNowMillisecond();
    m_working = false;
    m_gonnaClose = false;
    m_delayTime = 0;
    m_wannaClose = false;
}

void Socket::Tick(int64_t now, int interval) {
    m_now = now;
    if (m_wannaClose && m_delayTime >= 0) {
        m_delayTime -= interval;
        if (m_delayTime <= 0) {
            Shutdown();
            m_delayTime = -1;
        }
    }
}

void Socket::Dispatch() {}

void Socket::Retain() {
    ++m_reference;
}

void Socket::Release() {
    --m_reference;
}

void Socket::Destory() {
    m_destroyCallback(this);
}

void Socket::AttachSockId(const SockId_t& sockId) {
    m_sockId = sockId;
}

void Socket::DetachSockId() {
    if (m_sockId != INVALID_SOCKID) {
        DestroySockId(m_sockId);
        m_sockId = INVALID_SOCKID;
    }
}

void Socket::Disconnect(int delayTime) {
    if (!m_wannaClose) {
        m_delayTime = delayTime;
        m_wannaClose = true;
    }
}

void Socket::Shutdown() {
    if (m_sockId != INVALID_SOCKID) {
        ShutdownSockId(m_sockId, SHUT_WR);
    }
}

void Socket::Close() {
    m_working = false;
    m_gonnaClose = true;
}

void Socket::Start() {
    m_working = true;
    
    OnStart();
}

void Socket::Stop() {
    OnStop();

    DetachSockId();
    m_gonnaClose = false;
}

DECL_NAMESPACE_MZ_NET_END
