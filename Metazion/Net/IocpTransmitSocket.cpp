#include "Metazion/Net/IocpTransmitSocket.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include <Metazion/Share/Sync/AutoGuard.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

IocpTransmitSocket::IocpTransmitSocket()
    : m_socketBuffer(m_lock) {}

IocpTransmitSocket::~IocpTransmitSocket() {}

void IocpTransmitSocket::Reset() {
    IocpSocket::Reset();
    m_socketBuffer.Reset();
    m_sendOperation.Reset();
    m_recvOperation.Reset();
}

void IocpTransmitSocket::OnStarted() {
    IocpSocket::OnStarted();
    m_socketBuffer.Rework();
    m_sendOperation.Reset();
    m_recvOperation.Reset();
}

bool IocpTransmitSocket::IsClosing() {
    bool ret = IocpSocket::IsClosing();
    if (ret) {
        return true;
    }

    ret = m_sendOperation.IsBusy();
    if (ret) {
        return true;
    }

    ret = m_recvOperation.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

int IocpTransmitSocket::Send(const void* data, int length) {
    if (!IsIoAvailable()) {
        return 0;
    }

    m_lock.Lock();
    const int pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_lock.Unlock();

    if (pushLength < length) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return 0;
    }

    return pushLength;
}

int IocpTransmitSocket::SendNow(const void* data, int length) {
    if (!IsIoAvailable()) {
        return 0;
    }

    m_lock.Lock();
    const int pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_lock.Unlock();

    if (pushLength < length) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return 0;
    }

    PostOutputOperation();
    return pushLength;
}

bool IocpTransmitSocket::PostInputOperation() {
    if (!IsReady()) {
        return false;
    }

    if (m_recvOperation.IsBusy()) {
        return true;
    }

    DECL_BLOCK_BEGIN
    NS_SHARE::AutoGuard<NS_SHARE::MutexLock> autoGuard(m_lock);

    if (m_recvOperation.IsBusy()) {
        return true;
    }

    m_recvOperation.SetBusy(true);
    DECL_BLOCK_END

   return _PostInputOperation();
}

bool IocpTransmitSocket::PostOutputOperation() {
    if (!IsReady()) {
        return false;
    }

    if (m_sendOperation.IsBusy()) {
        return true;
    }

    if (!m_socketBuffer.HasDataToSend()) {
        return true;
    }

    DECL_BLOCK_BEGIN
    NS_SHARE::AutoGuard<NS_SHARE::MutexLock> autoGuard(m_lock);

    if (m_sendOperation.IsBusy()) {
        return true;
    }

    m_sendOperation.SetBusy(true);
    DECL_BLOCK_END

    return _PostOutputOperation();
}

bool IocpTransmitSocket::HandleSuccessOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    switch (iocpOperation->m_type) {
    case IocpOperation::TYPE_RECV:
        return HandleInputSuccessOperation(iocpOperation, byteNumber);
    case IocpOperation::TYPE_SEND:
        return HandleOutputSuccessOperation(iocpOperation, byteNumber);
    default: ASSERT_TRUE(false); return false;
    }
}

bool IocpTransmitSocket::HandleFailureOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    switch (iocpOperation->m_type) {
    case IocpOperation::TYPE_RECV:
        return HandleInputFailureOperation(iocpOperation, byteNumber, error);
    case IocpOperation::TYPE_SEND:
        return HandleOutputFailureOperation(iocpOperation, byteNumber, error);
    default: ASSERT_TRUE(false); return false;
    }
}

bool IocpTransmitSocket::HandleCloseOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    switch (iocpOperation->m_type) {
    case IocpOperation::TYPE_RECV:
        return HandleInputCloseOperation(iocpOperation, byteNumber);
    case IocpOperation::TYPE_SEND:
        return HandleOutputCloseOperation(iocpOperation, byteNumber);
    default: ASSERT_TRUE(false); return false;
    }
}

bool IocpTransmitSocket::_PostInputOperation() {
    const int recvLength = m_socketBuffer.m_recvBuffer.GetPushLength();
    char* recvBuffer = m_socketBuffer.m_recvBuffer.GetPushBuffer();

    m_recvOperation.m_wsaBuf.buf = recvBuffer;
    m_recvOperation.m_wsaBuf.len = recvLength;

    DWORD bytesRecvd = 0;
    DWORD flags = 0;
    const int ret = ::WSARecv(m_sockId
        , &m_recvOperation.m_wsaBuf
        , 1
        , &bytesRecvd
        , &flags
        , &m_recvOperation.m_overlapped
        , NULL);
    if (0 != ret) {
        const DWORD error = ::WSAGetLastError();
        if (ERROR_IO_PENDING != error) {
            HandleFailureOperation(&m_recvOperation, 0, error);
            return false;
        }
    }

    return true;
}

bool IocpTransmitSocket::_PostOutputOperation() {
    int sendLength = m_socketBuffer.m_sendBuffer.GetPullLength();
    if (sendLength <= 0) {
        sendLength = m_socketBuffer.PrepareSendBuffer();    
    }
    if (sendLength <= 0) {
        m_sendOperation.SetBusy(false);
        return true;
    }

    char* sendBuffer = m_socketBuffer.m_sendBuffer.GetPullBuffer();

    m_sendOperation.m_wsaBuf.buf = sendBuffer;
    m_sendOperation.m_wsaBuf.len = sendLength;

    DWORD bytesSent = 0;
    const int ret = ::WSASend(m_sockId
        , &m_sendOperation.m_wsaBuf
        , 1
        , &bytesSent
        , 0
        , &m_sendOperation.m_overlapped
        , NULL);
    if (0 != ret) {
        const DWORD error = ::WSAGetLastError();
        if (ERROR_IO_PENDING != error) {
            HandleFailureOperation(&m_sendOperation, 0, error);
            return false;
        }
    }

    return true;
}

bool IocpTransmitSocket::HandleInputSuccessOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    ASSERT_TRUE(&m_recvOperation == iocpOperation);

    if (0 == byteNumber) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return true;
    }

    m_socketBuffer.m_recvBuffer.JumpPushIndex(byteNumber);

    const char* recvData = m_socketBuffer.m_recvBuffer.GetPullBuffer();
    const int recvLength = m_socketBuffer.m_recvBuffer.GetPullLength();

    const int processLength = OnRecvData(recvData, recvLength);
    if (processLength < 0) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return false;
    }

    m_socketBuffer.m_recvBuffer.JumpPullIndex(processLength);
    m_socketBuffer.m_recvBuffer.Compact();

    return _PostInputOperation();
}

bool IocpTransmitSocket::HandleOutputSuccessOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    ASSERT_TRUE(&m_sendOperation == iocpOperation);

    if (0 == byteNumber) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return true;
    }

    const char* sendData = m_socketBuffer.m_sendBuffer.GetPullBuffer();
    const int sendLength = byteNumber;

    const int processLength = OnSendData(sendData, sendLength);
    if (processLength < 0) {
        ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Close();
        return false;
    }

    m_socketBuffer.m_sendBuffer.JumpPullIndex(processLength);
    m_socketBuffer.m_sendBuffer.Compact();

    return _PostOutputOperation();
}

bool IocpTransmitSocket::HandleInputFailureOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    ASSERT_TRUE(&m_recvOperation == iocpOperation);

    ::printf("Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
    Close();
    OnError(error);

    m_recvOperation.SetBusy(false);
    return true;
}

bool IocpTransmitSocket::HandleOutputFailureOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    ASSERT_TRUE(&m_sendOperation == iocpOperation);

    ::printf("Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
    Close();
    OnError(error);

    m_sendOperation.SetBusy(false);
    return true;
}

bool IocpTransmitSocket::HandleInputCloseOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    ASSERT_TRUE(&m_recvOperation == iocpOperation);

    ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
    Close();

    m_recvOperation.SetBusy(false);
    return true;
}

bool IocpTransmitSocket::HandleOutputCloseOperation(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    ASSERT_TRUE(&m_sendOperation == iocpOperation);

    ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
    Close();

    m_sendOperation.SetBusy(false);
    return true;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS
