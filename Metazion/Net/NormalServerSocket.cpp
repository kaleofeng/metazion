#include "Metazion/Net/NormalServerSocket.hpp"

#include "Metazion/Net/NormalSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

NormalServerSocket::NormalServerSocket() {}

NormalServerSocket::~NormalServerSocket() {}

void NormalServerSocket::Attach(const SockId_t& sockId) {
    AttachSockId(sockId);
}

void NormalServerSocket::OnConnected() {}

void NormalServerSocket::OnDisconnected() {}

int NormalServerSocket::OnSended(const void* data, int length) {
    return length;
}

int NormalServerSocket::OnRecved(const void* data, int length) {
    return length;
}


DECL_NAMESPACE_MZ_NET_END
