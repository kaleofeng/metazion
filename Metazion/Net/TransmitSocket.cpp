#include "Metazion/Net/TransmitSocket.hpp"

#include <Metazion/Share/Log/Logger.hpp>
#include <Metazion/Share/Time/Time.hpp>

#include "Metazion/Net/NetworkService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

TransmitSocket::TransmitSocket()
    : m_transmitStrategy(*this) {}

TransmitSocket::~TransmitSocket() {}

void TransmitSocket::Reset() {
    Socket::Reset();

    m_transmitStrategy.Reset();
    m_socketBuffer.Reset();
    m_remoteHost.Reset();

#if defined(MZ_ENABLE_STATISTIC)
    m_connectedTime = 0;
    m_disconnectedTime = 0;
    m_firstSendTime = 0;
    m_lastSendTime = 0;
    m_sendedBytes = 0;
    m_firstRecvTime = 0;
    m_lastRecvTime = 0;
    m_recvedBytes = 0;
#endif
}

void TransmitSocket::Prepare() {
    Socket::Prepare();

    m_socketBuffer.Prepare();
    m_transmitStrategy.Prepare();
}

bool TransmitSocket::IsAlive() const {
    auto ret = IsValid();
    if (ret) {
        return true;
    }

    ret = m_transmitStrategy.IsBusy();
    if (ret) {
        return true;
    }

    return false;
}

void TransmitSocket::OnAttached() {
    auto networkService = GetNetworkService();
    MZ_ASSERT_TRUE(!IsNull(networkService));

    auto& sendBufferPool = networkService->GetSendCachePool();
    m_socketBuffer.SetSendBufferPool(sendBufferPool);

    auto& recvBufferPool = networkService->GetRecvCachePool();
    m_socketBuffer.SetRecvCachePool(recvBufferPool);
}

void TransmitSocket::OnDetached() {}

void TransmitSocket::OnStart() {
    m_transmitStrategy.Start();

    OnConnected();
}

void TransmitSocket::OnStop() {
    OnDisconnected();
}

void TransmitSocket::OnError(int error) {}

int TransmitSocket::Send(const void* data, int length) {
    if (!IsWorking()) {
        return 0;
    }

    m_socketBuffer.m_sendLock.lock();
    const auto pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_socketBuffer.m_sendLock.unlock();

    if (pushLength < length) {
        NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Trace: socket close. [%s:%d]\n", __FILE__, __LINE__);
        Disconnect();
        return 0;
    }

    m_transmitStrategy.Launch();

    return pushLength;
}

void TransmitSocket::OnConnected() {
#if defined(MZ_ENABLE_STATISTIC)
    m_connectedTime = NS_MZ_SHARE::GetNowMillisecond();
#endif
}

void TransmitSocket::OnDisconnected() {
#if defined(MZ_ENABLE_STATISTIC)
    m_disconnectedTime = NS_MZ_SHARE::GetNowMillisecond();
#endif
}

void TransmitSocket::OnSended(const void* data, int length) {
#if defined(MZ_ENABLE_STATISTIC)
    const auto now = NS_MZ_SHARE::GetNowMillisecond();
    if (m_firstSendTime == 0) {
        m_firstSendTime = now;
    }

    m_lastSendTime = now;

    m_sendedBytes += length;
#endif
}

void TransmitSocket::OnRecved(const void* data, int length) {
#if defined(MZ_ENABLE_STATISTIC)
    const auto now = NS_MZ_SHARE::GetNowMillisecond();
    if (m_firstRecvTime == 0) {
        m_firstRecvTime = now;
    }

    m_lastRecvTime = now;

    m_recvedBytes += length;
#endif
}

DECL_NAMESPACE_MZ_NET_END
