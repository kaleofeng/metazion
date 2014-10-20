#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

Socket::Socket()
    : m_reference(0)
    , m_working(false)
    , m_wannaClose(false)
    , m_sockId(INVALID_SOCKID)
    , m_index(-1)
    , m_socketServer(nullptr) {}

Socket::~Socket() {}

void Socket::Reset() {
    m_reference = 0;
    m_working = false;
    m_wannaClose = false;
    m_sockId = INVALID_SOCKID;
    m_index = -1;
    m_socketServer = nullptr;
}

void Socket::Prepare() {
     m_working = false;
     m_wannaClose = false;
}

void Socket::Destory() {
    delete this;
}

void Socket::Tick(int interval) {}

void Socket::Dispatch() {}

void Socket::Retain() {
    LockGuard_t lockGuard(m_lock);
    
    ++m_reference;
}

void Socket::Release() {
    LockGuard_t lockGuard(m_lock);

    --m_reference;
}

void Socket::Start() {
    m_working = true;
    
    OnStart();
}

void Socket::Close() {
    m_working = false;
    m_wannaClose = true;
}

void Socket::DoClose() {
    OnClose();

    DetachSockId();
    m_wannaClose = false;
}

DECL_NAMESPACE_MZ_NET_END
