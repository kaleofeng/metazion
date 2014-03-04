#include "Metazion/Net/TransmitSocket.hpp"

#include "Metazion/Net/BaseSocketServer.hpp"

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

void TransmitSocket::OnAttached() {
    Socket::OnAttached();

    SocketServer* server = GetSocketServer();
    BaseSocketServer* baseServer = static_cast<BaseSocketServer*>(server);
    ASSERT_TRUE(!IsNull(baseServer));

    SocketBuffer::SendCache_t::BufferPool_t& sendBufferPool = baseServer->GetSendCachePool();
    m_socketBuffer.SetSendCachePool(sendBufferPool);

    SocketBuffer::RecvCache_t::BufferPool_t& recvBufferPool = baseServer->GetRecvCachePool();
    m_socketBuffer.SetRecvCachePool(recvBufferPool);
}

void TransmitSocket::OnStarted() {
    Socket::OnStarted();
    m_socketBuffer.Rework();
    m_transmitStrategy.Reset();
    OnConnected();
}

void TransmitSocket::OnClosed() {
    Socket::OnClosed();
    OnDisconnected();
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

void TransmitSocket::OnConnected() {}

void TransmitSocket::OnDisconnected() {}

int TransmitSocket::OnSendData(const void* data, int length) {
    return length;
}

int TransmitSocket::OnRecvData(const void* data, int length) {
    return length;
}

int TransmitSocket::Send(const void* data, int length) {
    if (!IsWorking()) {
        return 0;
    }

    m_socketBuffer.m_sendLock.Lock();
    const int pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_socketBuffer.m_sendLock.Unlock();

    if (pushLength < length) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return 0;
    }

    return pushLength;
}

int TransmitSocket::Recv(void* data, int length)
{
    m_socketBuffer.m_recvLock.Lock();
    const int pullLength = m_socketBuffer.m_recvCache.Pull(data, length);
    m_socketBuffer.m_recvLock.Unlock();
    
    return pullLength;
}

int TransmitSocket::Peek(void* data, int length)
{
    const int peekLength = m_socketBuffer.m_recvCache.Peek(data, length);
    return peekLength;
}

DECL_NAMESPACE_MZ_NET_END
