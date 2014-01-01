#include "Metazion/Net/TcpSocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

TcpSocketBuffer::TcpSocketBuffer(NS_SHARE::MutexLock& lock)
    : m_lock(lock)
    , m_sendCachePool(nullptr) {}

TcpSocketBuffer::~TcpSocketBuffer() {}

void TcpSocketBuffer::SetSendCachePool(SendCache_t::BufferPool_t& bufferPool) {
    m_sendCachePool = &bufferPool;
    m_sendCache.SetBufferPool(m_sendCachePool);
}

void TcpSocketBuffer::Reset() {
    m_sendCache.Reset();
    m_sendCachePool = nullptr;
    m_sendBuffer.Reset();
    m_recvBuffer.Reset();
}

void TcpSocketBuffer::Rework() {
    m_sendCache.Reset();
    m_sendCache.SetBufferPool(m_sendCachePool);
    m_sendBuffer.Reset();
    m_recvBuffer.Reset();
}

bool TcpSocketBuffer::HasDataToSend() const {
    if (m_sendBuffer.GetPullLength() > 0) {
        return true;
    }

    if (m_sendCache.GetPullLength() > 0) {
        return true;
    }

    return false;
}

int TcpSocketBuffer::PrepareSendBuffer() {
    m_sendBuffer.Reset();

    const int pushLength = m_sendBuffer.GetPushLength(); 
    char* pushBuffer = m_sendBuffer.GetPushBuffer();

    m_lock.Lock();
    const int realLength = m_sendCache.Pull(pushBuffer, pushLength);
    m_lock.Unlock();
    
    m_sendBuffer.SetPushIndex(realLength);
    return realLength;
}

DECL_NAMESPACE_MZ_NET_END
