#include "Metazion/Net/IocpListenStrategy.hpp"

#include <Metazion/Share/Log/Logger.hpp>

#include "Metazion/Net/ListenSocket.hpp"

#if defined(NETWORK_USE_IOCP_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

IocpListenStrategy::IocpListenStrategy(ListenSocket& listenSocket)
    : m_listenSocket(listenSocket) {}

IocpListenStrategy::~IocpListenStrategy() {}

void IocpListenStrategy::Reset() {
    m_acceptOperation.Reset();
    memset(m_acceptBuffer, 0, sizeof(m_acceptBuffer));
}

void IocpListenStrategy::Prepare() {
    Reset();
}

void IocpListenStrategy::Start() {
    PostInput();
}

bool IocpListenStrategy::IsBusy() const {
    return m_acceptOperation.IsBusy();
}

bool IocpListenStrategy::PostInput() {
    if (!m_listenSocket.IsWorking()) {
        return false;
    }

    if (m_acceptOperation.IsBusy()) {
        return true;
    }

    m_acceptOperation.SetBusy(true);

    return PostAccept();
}

bool IocpListenStrategy::PostOutput() {
    return true;
}

bool IocpListenStrategy::HandleSuccess(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    MZ_ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    const auto& acceptBuffer = m_acceptOperation.m_buffer;
    const auto& acceptSockId = m_acceptOperation.m_sockId;

    const auto localBuffer = m_acceptOperation.m_buffer;
    const auto localAddr = reinterpret_cast<const SockAddrIn_t*>(localBuffer);
    const auto remoteBuffer = m_acceptOperation.m_buffer + sizeof(SockAddrIn_t) + 16;
    const auto remoteAddr = reinterpret_cast<const SockAddrIn_t*>(remoteBuffer);

    Host peerHost;
    peerHost.FromSockAddrIn(remoteAddr);

    const auto& listenSockId = m_listenSocket.GetSockId();
    const auto ret = setsockopt(acceptSockId
        , SOL_SOCKET
        , SO_UPDATE_ACCEPT_CONTEXT
        , reinterpret_cast<const char*>(&listenSockId)
        , sizeof(listenSockId));
    if (ret != 0) {
        DestroySockId(acceptSockId);
    }

    if (!m_listenSocket.OnAccepted(acceptSockId, peerHost)) {
        DestroySockId(acceptSockId);
    }

    return PostAccept();
}

bool IocpListenStrategy::HandleFailure(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    MZ_ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Error: socket close. [%s:%d]\n", __FILE__, __LINE__);
    m_listenSocket.Close();
    m_listenSocket.OnError(error);

    DestroySockId(m_acceptOperation.m_sockId);
    m_acceptOperation.m_sockId = INVALID_SOCKID;
    m_acceptOperation.SetBusy(false);
    return true;
}

bool IocpListenStrategy::HandleClose(const IocpOperation* iocpOperation
    , DWORD byteNumber){
    MZ_ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Trace: socket close. [%s:%d]\n", __FILE__, __LINE__);
    m_listenSocket.Disconnect();

    DestroySockId(m_acceptOperation.m_sockId);
    m_acceptOperation.m_sockId = INVALID_SOCKID;
    m_acceptOperation.SetBusy(false);
    return true;
}

bool IocpListenStrategy::PostAccept() {
    const auto sockId = CreateSockId(TRANSPORT_TCP);
    if (sockId == INVALID_SOCKID) {
        m_acceptOperation.SetBusy(false);
        return false;
    }

    m_acceptOperation.m_buffer = m_acceptBuffer;
    m_acceptOperation.m_sockId = sockId;

    DWORD bytesRecvd = 0;
    const auto& listenSockId = m_listenSocket.GetSockId();
    const auto ret = AcceptEx(listenSockId
        , m_acceptOperation.m_sockId
        , m_acceptOperation.m_buffer
        , 0
        , sizeof(SockAddrIn_t) + 16
        , sizeof(SockAddrIn_t) + 16
        , &bytesRecvd
        , &m_acceptOperation.m_overlapped);
    if (ret == FALSE) {
        const DWORD error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            HandleFailure(&m_acceptOperation, 0, error);
            return false;
        }
    }

    return true;
}

DECL_NAMESPACE_MZ_NET_END

#endif
