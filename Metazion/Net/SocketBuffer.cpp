#include "Metazion/Net/SocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

SocketBuffer::SocketBuffer()
    : m_sendCachePool(nullptr)
    , m_recvCachePool(nullptr) {}

SocketBuffer::~SocketBuffer() {}

void SocketBuffer::Reset() {
    m_sendCachePool = nullptr;
    m_recvCachePool = nullptr;
    m_sendCache.Reset();
    m_recvCache.Reset();
    m_sendPlan.Reset();
    m_recvPlan.Reset();
}

void SocketBuffer::Prepare() {
    m_sendCache.Reset();
    m_sendCache.SetBufferPool(m_sendCachePool);
    m_recvCache.Reset();
    m_recvCache.SetBufferPool(m_recvCachePool);
    m_sendPlan.Reset();
    m_sendPlan.SetBufferPool(m_sendCachePool);
    m_recvPlan.Reset();
    m_recvPlan.SetBufferPool(m_recvCachePool);
}

void SocketBuffer::SetSendBufferPool(SendCache_t::BufferPool_t& bufferPool) {
    m_sendCachePool = &bufferPool;
    m_sendCache.SetBufferPool(m_sendCachePool);
    m_sendPlan.SetBufferPool(m_sendCachePool);
}

void SocketBuffer::SetRecvCachePool(RecvCache_t::BufferPool_t& bufferPool) {
    m_recvCachePool = &bufferPool;
    m_recvCache.SetBufferPool(m_recvCachePool);
    m_recvPlan.SetBufferPool(m_recvCachePool);
}

int SocketBuffer::SetupSendIov(IOV_TYPE iovs[NUMBER_SEND_IOV]) {
    auto index = 0;
    m_sendPlan.Traverse([&](SendPlan_t::BufferNode_t* buffer) {
        const auto ptr = buffer->m_value.GetPullBuffer();
        const auto len = buffer->m_value.GetPullLength();
        if (len < 1) {
            return true;
        }

        iovs[index].IOV_PTR_FIELD = ptr;
        iovs[index].IOV_LEN_FIELD = len;
        return ++index >= NUMBER_SEND_IOV;
    });
    return index;
}

int SocketBuffer::SetupRecvIov(IOV_TYPE iovs[NUMBER_RECV_IOV]) {
    auto index = 0;
    m_recvPlan.Traverse([&](SendPlan_t::BufferNode_t* buffer) {
        const auto ptr = buffer->m_value.GetPushBuffer();
        const auto len = buffer->m_value.GetPushLength();
        if (len < 1) {  
            return true;
        }

        iovs[index].IOV_PTR_FIELD = ptr;
        iovs[index].IOV_LEN_FIELD = len;
        return ++index >= NUMBER_RECV_IOV;
    });
    return index;
}

int SocketBuffer::PrepareSendPlan() {
    auto bufferNumber = m_sendPlan.GetStepSize();
    auto bufferLength = m_sendPlan.GetCapLength();

    m_sendLock.lock();

    while (bufferNumber < NUMBER_SEND_IOV && bufferLength < LENGTH_SEND_MAX) {
        if (m_sendPlan.IsMaximal()) {
            break;
        }

        auto buffer = m_sendCache.Detach();
        if (IsNull(buffer)) {
            break;
        }

        if (!m_sendPlan.Attach(buffer)) {
            break;
        }

        bufferNumber = m_sendPlan.GetStepSize();
        bufferLength = m_sendPlan.GetCapLength();
    }

    m_sendLock.unlock();
    return bufferLength;
}

int SocketBuffer::PrepareRecvPlan() {
    auto bufferNumber = m_recvPlan.GetStepSize();
    auto bufferLength = m_recvPlan.GetCapLength();

    if (bufferNumber < NUMBER_RECV_IOV && bufferLength < LENGTH_RECV_MAX) {
        const auto expandLength = m_recvPlan.Expand(LENGTH_RECV_MAX);
        MZ_UNUSED_VARIABLE(expandLength);

        bufferNumber = m_recvPlan.GetStepSize();
        bufferLength = m_recvPlan.GetCapLength();
    }

    return bufferLength;
}

int SocketBuffer::PreserveSendPlan(int length) {
    m_sendPlan.Skip(length);

    auto bufferNumber = m_sendPlan.GetStepSize();
    auto bufferLength = m_sendPlan.GetCurLength();

    MZ_UNUSED_VARIABLE(bufferNumber);
    return bufferLength;
}

int SocketBuffer::PreserveRecvPlan(int length) {
    m_recvPlan.Take(length);

    auto bufferNumber = m_recvPlan.GetStepSize();
    auto bufferLength = m_recvPlan.GetCurLength();

    m_recvLock.lock();

    while (bufferNumber > 0 && bufferLength > 0) {
        if (m_recvCache.IsMaximal()) {
            break;
        }

        auto buffer = m_recvPlan.Detach();
        if (IsNull(buffer)) {
            break;
        }

        if (!m_recvCache.Attach(buffer)) {
            break;
        }

        bufferNumber = m_recvPlan.GetStepSize();
        bufferLength = m_recvPlan.GetCurLength();
    }

    m_recvLock.unlock();
    return bufferLength;
}

bool SocketBuffer::HasDataToSend() const {
    if (m_sendPlan.GetCurLength() > 0) {
        return true;
    }

    if (m_sendCache.GetCurLength() > 0) {
        return true;
    }

    return false;
}

bool SocketBuffer::HasDataRecvedYet() const {
    if (m_recvCache.GetCurLength() > 0) {
        return true;
    }

    return false;
}

DECL_NAMESPACE_MZ_NET_END
