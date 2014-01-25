#include "Metazion/Net/NormalServerSocket.hpp"

#include "Metazion/Net/NormalSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

NormalServerSocket::NormalServerSocket() {}

NormalServerSocket::~NormalServerSocket() {}

void NormalServerSocket::Attach(const SockId_t& sockId) {
    AttachSockId(sockId);
}

void NormalServerSocket::OnAttached() {
    TransmitSocket::OnAttached();

    SocketServer* server = GetSocketServer();
    NormalSocketServer* normalServer = static_cast<NormalSocketServer*>(server);
    ASSERT_TRUE(!IsNull(normalServer));

    TcpSocketBuffer::SendCache_t::BufferPool_t& bufferPool = normalServer->GetSendCachePool();
    m_socketBuffer.SetSendCachePool(bufferPool);
}

void NormalServerSocket::OnStarted() {
    TransmitSocket::OnStarted();
    OnConnected();
}

void NormalServerSocket::OnClosed() {
    TransmitSocket::OnClosed();
    OnDisconnected();
}

void NormalServerSocket::OnConnected() {}

void NormalServerSocket::OnDisconnected() {}

DECL_NAMESPACE_MZ_NET_END
