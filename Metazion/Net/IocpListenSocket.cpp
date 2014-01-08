#include "Metazion/Net/IocpListenSocket.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include <Metazion/Share/Sync/AutoGuard.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

IocpListenSocket::IocpListenSocket() {
    ::memset(m_acceptBuffer, 0, sizeof(m_acceptBuffer));
}

IocpListenSocket::~IocpListenSocket() {}

void IocpListenSocket::Reset() {
    IocpSocket::Reset();
    m_localHost.Reset();
    m_acceptOperation.Reset();
    ::memset(m_acceptBuffer, 0, sizeof(m_acceptBuffer));
}

void IocpListenSocket::OnStarted() {
    IocpSocket::OnStarted();
    m_acceptOperation.Reset();
    ::memset(m_acceptBuffer, 0, sizeof(m_acceptBuffer));
}

bool IocpListenSocket::IsClosing() {
    bool ret = IocpSocket::IsClosing();
    if (ret) {
        return true;
    }

    ret = m_acceptOperation.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

void IocpListenSocket::SetLocalHost(const char* ip, int port) {
    m_localHost.SetFamily(AF_INET);
    m_localHost.SetIp(ip);
    m_localHost.SetPort(port);
}

bool IocpListenSocket::Listen(int backlog) {
    const SockId_t sockId = CreateSockId(TRANSPORT_TCP);
    if (INVALID_SOCKID == sockId) {
        return false;
    }

    int optValue = 1;
    SockLen_t optLength = sizeof(optValue);
    int ret = SetSockOpt(sockId, SOL_SOCKET, SO_REUSEADDR, &optValue, optLength);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    SockAddr_t* sockAddr = m_localHost.SockAddr();
    SockLen_t sockAddrLen = m_localHost.SockAddrLen();
    ret = ::bind(sockId, sockAddr, sockAddrLen);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    ret = ::listen(sockId, backlog);
    if (SOCKET_ERROR == ret) {
        DestroySockId(sockId);
        return false;
    }

    OpenSockId(sockId);
    return true;
}

bool IocpListenSocket::PostInputOperation() {
    if (!IsReady()) {
        return false;
    }

    if (m_acceptOperation.IsBusy()) {
        return true;
    }

    DECL_BLOCK_BEGIN
    NS_SHARE::AutoGuard<NS_SHARE::MutexLock> autoGuard(m_lock);

    if (m_acceptOperation.IsBusy()) {
        return true;
    }

    m_acceptOperation.SetBusy(true);
    DECL_BLOCK_END

    return _PostAcceptOperation();
}

bool IocpListenSocket::PostOutputOperation() {
    return true;
}

bool IocpListenSocket::HandleSuccessOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    const SockId_t& sockId = m_acceptOperation.m_sockId;

    const int ret = ::setsockopt(sockId
        , SOL_SOCKET
        , SO_UPDATE_ACCEPT_CONTEXT
        , reinterpret_cast<const char*>(&m_sockId)
        , sizeof(m_sockId));
    if (0 != ret) {
       DestroySockId(sockId);
    }

    if (!OnAccepted(sockId)) {
        DestroySockId(sockId);
    }

    return _PostAcceptOperation();
}

bool IocpListenSocket::HandleFailureOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    ::printf("Socket Error: socket close. [%s:%d]\n", __FILE__, __LINE__);
    Close();
    OnError(error);

    DestroySockId(m_acceptOperation.m_sockId);
    m_acceptOperation.m_sockId = INVALID_SOCKID;
    m_acceptOperation.SetBusy(false);
    return true;
}

bool IocpListenSocket::HandleCloseOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber){
    ASSERT_TRUE(&m_acceptOperation == iocpOperation);

    ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
    Close();

    DestroySockId(m_acceptOperation.m_sockId);
    m_acceptOperation.m_sockId = INVALID_SOCKID;
    m_acceptOperation.SetBusy(false);
    return true;
}

bool IocpListenSocket::_PostAcceptOperation() {
    const SockId_t sockId = CreateSockId(TRANSPORT_TCP);
    if (INVALID_SOCKID == sockId) {
        m_acceptOperation.SetBusy(false);
        return false;
    }

    m_acceptOperation.m_buffer = m_acceptBuffer;
    m_acceptOperation.m_sockId = sockId;

    DWORD bytesRecvd = 0;
    const BOOL ret = ::AcceptEx(m_sockId
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
