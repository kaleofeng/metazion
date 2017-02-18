#include "Metazion/Net/IocpTransmitStrategy.hpp"

#include <Metazion/Share/Log/Logger.hpp>

#include "Metazion/Net/TransmitSocket.hpp"

#if defined(NETWORK_USE_IOCP_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

IocpTransmitStrategy::IocpTransmitStrategy(TransmitSocket& transmitSocket)
    : m_transmitSocket(transmitSocket) {}

IocpTransmitStrategy::~IocpTransmitStrategy() {}

void IocpTransmitStrategy::Reset() {
    m_sendOperation.Reset();
    m_recvOperation.Reset();
}

void IocpTransmitStrategy::Prepare() {
    Reset();
}

void IocpTransmitStrategy::Start() {
    PostInput();
}

void IocpTransmitStrategy::Launch() {
    PostOutput();
}

bool IocpTransmitStrategy::IsBusy() const {
    auto ret = m_sendOperation.IsBusy();
    if (ret) {
        return true;
    }

    ret = m_recvOperation.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

bool IocpTransmitStrategy::PostInput() {
    if (!m_transmitSocket.IsWorking()) {
        return false;
    }

    if (m_recvOperation.IsBusy()) {
        return true;
    }

    m_recvOperation.SetBusy(true);

   return PostRecv();
}

bool IocpTransmitStrategy::PostOutput() {
    if (!m_transmitSocket.IsWorking()) {
        return false;
    }

    if (m_sendOperation.IsBusy()) {
        return true;
    }

    if (!m_transmitSocket.GetSocketBuffer().HasDataToSend()) {
        return true;
    }

    m_sendOperation.SetBusy(true);

    return PostSend();
}

bool IocpTransmitStrategy::HandleSuccess(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    switch (iocpOperation->m_type) {
    case IocpOperation::TYPE_RECV:
        return HandleRecvSuccess(iocpOperation, byteNumber);
    case IocpOperation::TYPE_SEND:
        return HandleSendSuccess(iocpOperation, byteNumber);
    default: MZ_ASSERT_TRUE(false); return false;
    }
}

bool IocpTransmitStrategy::HandleFailure(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    switch (iocpOperation->m_type) {
    case IocpOperation::TYPE_RECV:
        return HandleRecvFailure(iocpOperation, byteNumber, error);
    case IocpOperation::TYPE_SEND:
        return HandleSendFailure(iocpOperation, byteNumber, error);
    default: MZ_ASSERT_TRUE(false); return false;
    }
}

bool IocpTransmitStrategy::HandleClose(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    switch (iocpOperation->m_type) {
    case IocpOperation::TYPE_RECV:
        return HandleRecvClose(iocpOperation, byteNumber);
    case IocpOperation::TYPE_SEND:
        return HandleSendClose(iocpOperation, byteNumber);
    default: MZ_ASSERT_TRUE(false); return false;
    }
}

bool IocpTransmitStrategy::PostRecv() {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    const auto recvLength = socketBuffer.PrepareRecvPlan();

    const auto count = socketBuffer.SetupRecvIov(m_recvOperation.m_iov);

    DWORD bytesRecvd = 0;
    DWORD flags = 0;
    const auto& transmitSockId = m_transmitSocket.GetSockId();
    const auto ret = WSARecv(transmitSockId
        , m_recvOperation.m_iov
        , count
        , &bytesRecvd
        , &flags
        , &m_recvOperation.m_overlapped
        , NULL);
    if (ret != 0) {
        const auto error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            HandleFailure(&m_recvOperation, 0, error);
            return false;
        }
    }

    return true;
}

bool IocpTransmitStrategy::PostSend() {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    const auto sendLength = socketBuffer.PrepareSendPlan();
    if (sendLength < 1) {
        m_sendOperation.SetBusy(false);
        return true;
    }

    const auto count = socketBuffer.SetupSendIov(m_sendOperation.m_iov);

    DWORD bytesSent = 0;
    const auto& transmitSockId = m_transmitSocket.GetSockId();
    const auto ret = WSASend(transmitSockId
        , m_sendOperation.m_iov
        , count
        , &bytesSent
        , 0
        , &m_sendOperation.m_overlapped
        , NULL);
    if (ret != 0) {
        const auto error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            HandleFailure(&m_sendOperation, 0, error);
            return false;
        }
    }

    return true;
}

bool IocpTransmitStrategy::HandleRecvSuccess(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    MZ_ASSERT_TRUE(&m_recvOperation == iocpOperation);

    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    const auto planLength = socketBuffer.m_recvPlan.GetCurLength();
    const auto cacheLength = socketBuffer.m_recvCache.GetCurLength();
    //NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Recv: plan[%d] cache[%d]. [%s:%d]\n", planLength, cacheLength, __FILE__, __LINE__);

    if (byteNumber == 0) {
        NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        m_transmitSocket.Close();
        return true;
    }

    const auto recvData = nullptr;
    const auto recvLength = byteNumber;

    m_transmitSocket.OnRecved(recvData, recvLength);

    const auto restLength = socketBuffer.PreserveRecvPlan(byteNumber);
    if (restLength > 0) {
        NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Trace: socket close. [%s:%d]\n", __FILE__, __LINE__);
        m_transmitSocket.Disconnect();
        return false;
    }

    return PostRecv();
}

bool IocpTransmitStrategy::HandleSendSuccess(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    MZ_ASSERT_TRUE(&m_sendOperation == iocpOperation);

    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();
    
    const auto planLength = socketBuffer.m_sendPlan.GetCurLength();
    const auto cacheLength = socketBuffer.m_sendCache.GetCurLength();
    //NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Send: plan[%d] cache[%d]. [%s:%d]\n", planLength, cacheLength, __FILE__, __LINE__);

    if (byteNumber == 0) {
        NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
        m_transmitSocket.Close();
        return true;
    }

    const auto sendData = nullptr;
    const auto sendLength = byteNumber;

    m_transmitSocket.OnSended(sendData, sendLength);

    const auto restLength = socketBuffer.PreserveSendPlan(byteNumber);
    MZ_UNUSED_VARIABLE(restLength);

    return PostSend();
}

bool IocpTransmitStrategy::HandleRecvFailure(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    MZ_ASSERT_TRUE(&m_recvOperation == iocpOperation);

    NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
    m_transmitSocket.Close();
    m_transmitSocket.OnError(error);

    m_recvOperation.SetBusy(false);
    return true;
}

bool IocpTransmitStrategy::HandleSendFailure(const IocpOperation* iocpOperation
    , DWORD byteNumber, int error) {
    MZ_ASSERT_TRUE(&m_sendOperation == iocpOperation);

    NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
    m_transmitSocket.Close();
    m_transmitSocket.OnError(error);

    m_sendOperation.SetBusy(false);
    return true;
}

bool IocpTransmitStrategy::HandleRecvClose(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    MZ_ASSERT_TRUE(&m_recvOperation == iocpOperation);

    NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Trace: socket close. [%s:%d]\n", __FILE__, __LINE__);
    m_transmitSocket.Disconnect();

    m_recvOperation.SetBusy(false);
    return true;
}

bool IocpTransmitStrategy::HandleSendClose(const IocpOperation* iocpOperation
    , DWORD byteNumber) {
    MZ_ASSERT_TRUE(&m_sendOperation == iocpOperation);

    NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Trace: socket close. [%s:%d]\n", __FILE__, __LINE__);
    m_transmitSocket.Disconnect();

    m_sendOperation.SetBusy(false);
    return true;
}

DECL_NAMESPACE_MZ_NET_END

#endif
