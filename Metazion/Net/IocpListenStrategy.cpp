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

bool IocpListenStrategy::IsBusy() const {
    return m_acceptOperation.IsBusy();
}

bool IocpListenStrategy::PostInputOperation() {
    if (!m_listenSocket.IsReady()) {
        return false;
    }

    if (m_acceptOperation.IsBusy()) {
        return true;
    }

    m_acceptOperation.SetBusy(true);

    return _PostAcceptOperation();
}

bool IocpListenStrategy::PostOutputOperation() {
    return true;
}

bool IocpListenStrategy::HandleSuccessOperation(const IocpOperation* iocpOperation
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

    return _PostAcceptOperation();
}

bool IocpListenStrategy::HandleFailureOperation(const IocpOperation* iocpOperation
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

bool IocpListenStrategy::HandleCloseOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber){
    ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
    m_listenSocket.Close();

    DestroySockId(m_acceptOperation.m_sockId);
    m_acceptOperation.m_sockId = INVALID_SOCKID;
    m_acceptOperation.SetBusy(false);
    return true;
}

bool IocpListenStrategy::_PostAcceptOperation() {
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
            HandleFailureOperation(&m_acceptOperation, 0, error);
            return false;
        }
    }

    return true;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS
