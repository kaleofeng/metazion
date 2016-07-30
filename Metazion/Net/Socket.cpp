#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

Socket::Socket() {}

Socket::~Socket() {}

void Socket::Reset() {
    m_reference = 0;
    m_working = false;
    m_gonnaClose = false;
    m_wannaClose = false;
    m_sockId = INVALID_SOCKID;
    m_index = -1;
    m_networkService = nullptr;
    m_destroyCallback = [](Socket* socket) { delete socket; };
}

void Socket::Prepare() {
     m_working = false;
     m_gonnaClose = false;
     m_wannaClose = false;
}

void Socket::Tick(int interval) {}

void Socket::Dispatch() {}

void Socket::Retain() {
    ++m_reference;
}

void Socket::Release() {
    --m_reference;
}

void Socket::Close() {
    m_working = false;
    m_gonnaClose = true;
}

void Socket::Disconnect() {
    DetachSockId();
    m_wannaClose = true;
}

void Socket::Destory() {
    m_destroyCallback(this);
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
