#include "Metazion/Net/IocpListenStrategy.hpp"

#include "Metazion/Net/ListenSocket.hpp"

#if defined(MZ_PLATFORM_WINOWS)

DECL_NAMESPACE_MZ_NET_BEGIN

IocpListenStrategy::IocpListenStrategy(ListenSocket& listenSocket)
    : m_listenSocket(listenSocket) {
    ::memset(m_acceptBuffer, 0, sizeof(m_acceptBuffer));
}

IocpListenStrategy::~IocpListenStrategy() {}

void IocpListenStrategy::Reset() {
    m_acceptOperation.Reset();
    ::memset(m_acceptBuffer, 0, sizeof(m_acceptBuffer));
}

void IocpListenStrategy::OnStarted() {
    Reset();
    Input();
}

bool IocpListenStrategy::IsBusy() const {
    return m_acceptOperation.IsBusy();
}

bool IocpListenStrategy::Input() {
    if (!m_listenSocket.IsReady()) {
        return false;
    }

    if (m_acceptOperation.IsBusy()) {
        return true;
    }

    m_acceptOperation.SetBusy(true);

    return PostAccept();
}

bool IocpListenStrategy::Output() {
    return true;
}

bool IocpListenStrategy::OnSuccess(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    const auto& acceptSockId = m_acceptOperation.m_sockId;

    const auto& listenSockId = m_listenSocket.GetSockId();
    const auto ret = ::setsockopt(acceptSockId
        , SOL_SOCKET
        , SO_UPDATE_ACCEPT_CONTEXT
        , reinterpret_cast<const char*>(&listenSockId)
        , sizeof(listenSockId));
    if (0 != ret) {
        DestroySockId(acceptSockId);
    }

    if (!m_listenSocket.OnAccepted(acceptSockId)) {
        DestroySockId(acceptSockId);
    }

    return PostAccept();
}

bool IocpListenStrategy::OnFailure(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    ::printf("Socket Error: socket close. [%s:%d]\n", __FILE__, __LINE__);
    m_listenSocket.Close();
    m_listenSocket.OnError(error);

    DestroySockId(m_acceptOperation.m_sockId);
    m_acceptOperation.m_sockId = INVALID_SOCKID;
    m_acceptOperation.SetBusy(false);
    return true;
}

bool IocpListenStrategy::OnClose(const IocpOperation* iocpOperation
    , DWORD byteNumber){
    ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
    m_listenSocket.Close();

    DestroySockId(m_acceptOperation.m_sockId);
    m_acceptOperation.m_sockId = INVALID_SOCKID;
    m_acceptOperation.SetBusy(false);
    return true;
}

bool IocpListenStrategy::PostAccept() {
    const auto sockId = CreateSockId(TRANSPORT_TCP);
    if (INVALID_SOCKID == sockId) {
        m_acceptOperation.SetBusy(false);
        return false;
    }

    m_acceptOperation.m_buffer = m_acceptBuffer;
    m_acceptOperation.m_sockId = sockId;

    DWORD bytesRecvd = 0;
    const auto& listenSockId = m_listenSocket.GetSockId();
    const auto ret = ::AcceptEx(listenSockId
        , m_acceptOperation.m_sockId
        , m_acceptOperation.m_buffer
        , 0
        , sizeof(SockAddrIn_t) + 16
        , sizeof(SockAddrIn_t) + 16
        , &bytesRecvd
        , &m_acceptOperation.m_overlapped);
    if (FALSE == ret) {
        const DWORD error = ::WSAGetLastError();
        if (ERROR_IO_PENDING != error) {
            OnFailure(&m_acceptOperation, 0, error);
            return false;
        }
    }

    return true;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS
