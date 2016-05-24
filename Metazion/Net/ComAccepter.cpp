#include "Metazion/Net/ComAccepter.hpp"

#include "Metazion/Net/ListenSocket.hpp"
#include "Metazion/Net/AppServerSocket.hpp"
#include "Metazion/Net/NetworkService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

ComAccepter::ComAccepter(ListenSocket& socket)
    : m_socket(socket) {}

ComAccepter::~ComAccepter() {}

void ComAccepter::Reset() {
    SetCreateSocketCallback(nullptr);
}

bool ComAccepter::Accept(const SockId_t& sockId) {
    NetworkService* networkService = m_socket.GetNetworkService();
    MZ_ASSERT_TRUE(!IsNull(networkService));

    auto socket = m_createServerSocketCallback();
    MZ_ASSERT_TRUE(!IsNull(socket));

    socket->Attach(sockId);

    const bool ret = networkService->Manage(socket);
    if (!ret) {
        socket->Destory();
        return false;
    }

    return true;
}

DECL_NAMESPACE_MZ_NET_END
