#include "Metazion/Net/NormalListenSocket.hpp"

#include "Metazion/Net/NormalServerSocket.hpp"
#include "Metazion/Net/NormalSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

NormalListenSocket::NormalListenSocket() {}

NormalListenSocket::~NormalListenSocket() {}

int NormalListenSocket::GetType() const {
    return SOCKET_TCP_LISTEN;
}

bool NormalListenSocket::OnAccepted(const SockId_t& sockId) {
    BaseListenSocket::OnAccepted(sockId);

    NormalServerSocket* socket = CreateServerSocket();
    ASSERT_TRUE(!IsNull(socket));

    socket->Attach(sockId);
    
    const bool ret = m_socketServer->Attach(socket);
    if (!ret) {
        socket->Destory();
        return false;
    }

    return true;
}

DECL_NAMESPACE_MZ_NET_END