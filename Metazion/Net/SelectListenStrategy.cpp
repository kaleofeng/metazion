#include "Metazion/Net/SelectListenStrategy.hpp"

#include <Metazion/Share/Log/Logger.hpp>

#include "Metazion/Net/ListenSocket.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

SelectListenStrategy::SelectListenStrategy(ListenSocket& listenSocket)
    : m_listenSocket(listenSocket) {}

SelectListenStrategy::~SelectListenStrategy() {}

void SelectListenStrategy::Reset() {
    // Nothing to do.
}

void SelectListenStrategy::Prepare() {
    Reset();
}

void SelectListenStrategy::Start() {
    // Nothing to do.
}

bool SelectListenStrategy::IsBusy() const {
    return false;
}

void SelectListenStrategy::PostInput() {
    Host peerHost;
    auto sockAddr = peerHost.SockAddr();
    auto sockAddrLen = peerHost.SockAddrLen();
    const auto& listenSockId = m_listenSocket.GetSockId();
    while (true) {
        auto sockId = accept(listenSockId, sockAddr, &sockAddrLen);
        if (sockId == INVALID_SOCKID) {
            const auto error = SAGetLastError();
            if (IsInterrupted(error)) {
                continue;
            }

            if (IsWouldBlock(error)) {
               break;
            }

            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            m_listenSocket.Close();
            m_listenSocket.OnError(error);
            break;
        }

        SetNonBlock(sockId);

        if (!m_listenSocket.OnAccepted(sockId, peerHost)) {
            DestroySockId(sockId);
        }
    }
}

void SelectListenStrategy::PostOutput() {
    // Nothing to do.
}

void SelectListenStrategy::EnableOutput() {
    // Nothing to do.
}

DECL_NAMESPACE_MZ_NET_END

#endif
