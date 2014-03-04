#include "Metazion/Net/NormalServerSocket.hpp"

#include "Metazion/Net/NormalSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

NormalServerSocket::NormalServerSocket() {}

NormalServerSocket::~NormalServerSocket() {}

void NormalServerSocket::Attach(const SockId_t& sockId) {
    AttachSockId(sockId);
}

DECL_NAMESPACE_MZ_NET_END
