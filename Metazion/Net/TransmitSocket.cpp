#include "Metazion/Net/TransmitSocket.hpp"

#include "Metazion/Net/GenericSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

TransmitSocket::TransmitSocket()
    : m_transmitStrategy(*this) {}

TransmitSocket::~TransmitSocket() {}

IoStrategy& TransmitSocket::GetIoStrategy() {
    return m_transmitStrategy;
}

void TransmitSocket::Reset() {
    Socket::Reset();
    m_socketBuffer.Reset();
    m_transmitStrategy.Reset();
}

void TransmitSocket::Prepare() {
    Socket::Prepare();
    m_socketBuffer.Prepare();
    m_transmitStrategy.Prepare();
}

void TransmitSocket::OnAttached() {
    Socket::OnAttached();

    auto server = GetSocketServer();
    auto genericServer = static_cast<GenericSocketServer*>(server);
    ASSERT_TRUE(!IsNull(genericServer));

    auto& sendBufferPool = genericServer->GetSendCachePool();
    m_socketBuffer.SetSendCachePool(sendBufferPool);

    auto& recvBufferPool = genericServer->GetRecvCachePool();
    m_socketBuffer.SetRecvCachePool(recvBufferPool);
}

void TransmitSocket::OnStarted() {
    Socket::OnStarted();
    m_transmitStrategy.Start();
    OnConnected();
}

void TransmitSocket::OnClosed() {
    Socket::OnClosed();
    OnDisconnected();
}

bool TransmitSocket::IsAlive() const {
    auto ret = Socket::IsAlive();
    if (ret) {
        return true;
    }

    ret = m_transmitStrategy.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

void TransmitSocket::OnConnected() {}

void TransmitSocket::OnDisconnected() {}

int TransmitSocket::OnSended(const void* data, int length) {
    return length;
}

int TransmitSocket::OnRecved(const void* data, int length) {
    return length;
}

int TransmitSocket::Send(const void* data, int length) {
    if (!IsWorking()) {
        return 0;
    }

    m_socketBuffer.m_sendLock.lock();
    const auto pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_socketBuffer.m_sendLock.unlock();

    if (pushLength < length) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return 0;
    }

    return pushLength;
}

DECL_NAMESPACE_MZ_NET_END
