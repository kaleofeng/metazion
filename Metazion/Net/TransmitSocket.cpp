#include "Metazion/Net/TransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

TransmitSocket::TransmitSocket()
    : m_socketBuffer(m_lock)
    , m_transmitStrategy(*this) {}

TransmitSocket::~TransmitSocket() {}

void TransmitSocket::Reset() {
    Socket::Reset();
    m_socketBuffer.Reset();
    m_transmitStrategy.Reset();
}

void TransmitSocket::OnStarted() {
    Socket::OnStarted();
    m_socketBuffer.Rework();
    m_transmitStrategy.Reset();
}

bool TransmitSocket::IsAlive() const {
    bool ret = Socket::IsAlive();
    if (ret) {
        return true;
    }

    ret = m_transmitStrategy.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

int TransmitSocket::Send(const void* data, int length) {
    if (!IsWorking()) {
        return 0;
    }

    m_lock.Lock();
    const int pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_lock.Unlock();

    if (pushLength < length) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return 0;
    }

    return pushLength;
}

DECL_NAMESPACE_MZ_NET_END
