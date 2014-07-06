#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

Socket::Socket()
    : m_reference(0)
    , m_working(false)
    , m_sockId(INVALID_SOCKID)
    , m_index(-1)
    , m_socketServer(nullptr) {}

Socket::~Socket() {}

void Socket::Reset() {
    m_reference = 0;
    m_working = false;
    m_sockId = INVALID_SOCKID;
    m_index = -1;
    m_socketServer = nullptr;
}

void Socket::Destory() {
    delete this;
}

void Socket::Tick(int interval) {}

void Socket::Dispatch() {}

void Socket::OnAttached() {}

void Socket::OnDetached() {}

void Socket::OnStarted() {}

void Socket::OnClosed() {}

bool Socket::OnError(int error) {
    return true;
}

bool Socket::IsAlive() const {
    if (IsValid()) {
        return true;
    }

    return false;
}

bool Socket::IsActive() const {
    if (!IsReady()) {
        return false;
    }

    return true;
}

bool Socket::IsClosed() const {
    if (IsReady()) {
        return false;
    }

    return true;
}

void Socket::Start() {
    OnStarted();
    m_working = true;
}

void Socket::Close() {
    if (INVALID_SOCKID == m_sockId) {
        return;
    }

    m_lock.lock();
    if (INVALID_SOCKID == m_sockId) {
        m_lock.unlock();
        return;
    }

    m_working = false;
    DetachSockId();
    m_lock.unlock();

    OnClosed();
}

void Socket::Retain() {
    m_lock.lock();
    ++m_reference;
    m_lock.unlock();
}

void Socket::Release() {
    m_lock.lock();
    --m_reference;
    m_lock.unlock();
}

void Socket::AttachSockId(const SockId_t& sockId) {
    m_sockId = sockId;
}

void Socket::DetachSockId() {
    if (INVALID_SOCKID != m_sockId) {
        DestroySockId(m_sockId);
        m_sockId = INVALID_SOCKID;
    }
}

DECL_NAMESPACE_MZ_NET_END
