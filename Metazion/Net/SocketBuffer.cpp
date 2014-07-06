#include "Metazion/Net/SocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

SocketBuffer::SocketBuffer()
    : m_sendCachePool(nullptr)
    , m_recvCachePool(nullptr) {}

SocketBuffer::~SocketBuffer() {}

void SocketBuffer::SetSendCachePool(SendCache_t::BufferPool_t& bufferPool) {
    m_sendCachePool = &bufferPool;
    m_sendCache.SetBufferPool(m_sendCachePool);
}

void SocketBuffer::SetRecvCachePool(RecvCache_t::BufferPool_t& bufferPool) {
    m_recvCachePool = &bufferPool;
    m_recvCache.SetBufferPool(m_recvCachePool);
}

void SocketBuffer::Reset() {
    m_sendCache.Reset();
    m_sendCachePool = nullptr;
    m_recvCache.Reset();
    m_recvCachePool = nullptr;
    m_sendBuffer.Reset();
    m_recvBuffer.Reset();
}

void SocketBuffer::Rework() {
    m_sendCache.Reset();
    m_sendCache.SetBufferPool(m_sendCachePool);
    m_recvCache.Reset();
    m_recvCache.SetBufferPool(m_recvCachePool);
    m_sendBuffer.Reset();
    m_recvBuffer.Reset();
}

int SocketBuffer::PrepareSendBuffer() {
    m_sendBuffer.Reset();

    auto pushBuffer = m_sendBuffer.GetPushBuffer();
    const auto pushLength = m_sendBuffer.GetPushLength();

    m_sendLock.lock();
    const auto pullLength = m_sendCache.Pull(pushBuffer, pushLength);
    m_sendLock.unlock();
    
    m_sendBuffer.SetPushIndex(pullLength);
    return pullLength;
}

int SocketBuffer::PreserveRecvBuffer() {
    const auto pullLength = m_recvBuffer.GetPullLength();
    const auto pullBuffer = m_recvBuffer.GetPullBuffer();

    m_recvLock.lock();
    const auto pushLength = m_recvCache.Push(pullBuffer, pullLength);
    m_recvLock.unlock();

    m_recvBuffer.JumpPullIndex(pushLength);
    m_recvBuffer.Compact();
    return pushLength;
}

bool SocketBuffer::HasDataToSend() const {
    if (m_sendBuffer.GetPullLength() > 0) {
        return true;
    }

    if (m_sendCache.GetPullLength() > 0) {
        return true;
    }

    return false;
}

DECL_NAMESPACE_MZ_NET_END
