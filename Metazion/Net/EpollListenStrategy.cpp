#include "Metazion/Net/EpollListenStrategy.hpp"

#include "Metazion/Net/ListenSocket.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_NET_BEGIN

EpollListenStrategy::EpollListenStrategy(ListenSocket& listenSocket)
    : m_listenSocket(listenSocket) {}

EpollListenStrategy::~EpollListenStrategy() {}

void EpollListenStrategy::Reset() {
    // Nothing to do.
}

void EpollListenStrategy::Prepare() {
    Reset();
}

void EpollListenStrategy::Tick(int interval) {
    // Nothing to do.
}

void EpollListenStrategy::Start() {
    // Nothing to do.
}

bool EpollListenStrategy::IsBusy() const {
    return false;
}

void EpollListenStrategy::DoInput() {
    Host peerHost;
    auto sockAddr = peerHost.SockAddr();
    auto sockAddrLen = peerHost.SockAddrLen();
    const auto& listenSockId = m_listenSocket.GetSockId();
    while (true) {
        auto sockId = ::accept(listenSockId, sockAddr, &sockAddrLen);
        if (sockId == INVALID_SOCKID) {
            const auto error = GetLastError();
            if (error == EINTR) {
                continue;
            }
            if (error == EAGAIN) {
               break;
            }

            switch(error) {
            case EMFILE:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case EPERM:
                ASSERT_TRUE(false);
                break;
            default: break;
            };

            ::printf("Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            m_listenSocket.Close();
            m_listenSocket.OnError(error);
            break;
        }

        SetNonBlock(sockId);

        if (!m_listenSocket.OnAccepted(sockId)) {
            DestroySockId(sockId);
        }
    }
}

void EpollListenStrategy::DoOutput() {
    // Nothing to do.
}

void EpollListenStrategy::EnableOutput() {
    // Nothing to do.
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
