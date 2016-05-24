#include "Metazion/Net/EpollTransmitStrategy.hpp"

#include <Metazion/Share/Log/Logger.hpp>

#include "Metazion/Net/TransmitSocket.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

EpollTransmitStrategy::EpollTransmitStrategy(TransmitSocket& transmitSocket)
    : m_transmitSocket(transmitSocket) {}

EpollTransmitStrategy::~EpollTransmitStrategy() {}

void EpollTransmitStrategy::Reset() {
    m_sendOperation.Reset();
    m_recvOperation.Reset();
    m_canOutput = false;
}

void EpollTransmitStrategy::Prepare() {
    Reset();
}

void EpollTransmitStrategy::Start() {
    // Nothing to do.
}

bool EpollTransmitStrategy::IsBusy() const {
    return false;
}

void EpollTransmitStrategy::PostInput() {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    const auto& transmitSockId = m_transmitSocket.GetSockId();
    while (true) {
        const auto pushLength = socketBuffer.PrepareRecvPlan();
        MZ_UNUSED_VARIABLE(pushLength);

        const auto count = socketBuffer.SetupRecvIov(m_recvOperation.m_iov);

        const auto recvLength = readv(transmitSockId, m_recvOperation.m_iov, count);
        if (recvLength == 0) {
            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            m_transmitSocket.Close();
            break;
        }
        else if (recvLength < 0) {
            const auto error = SAGetLastError();
            if (IsInterrupted(error)) {
                continue;
            }

            if (IsWouldBlock(error)) {
                break;
            }

            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            m_transmitSocket.Close();
            m_transmitSocket.OnError(error);
            break;
        }

        m_transmitSocket.OnRecved(nullptr, recvLength);

        const auto restLength = socketBuffer.PreserveRecvPlan(recvLength);
        if (restLength > 0) {
            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            m_transmitSocket.Close();
        }
    }
}

void EpollTransmitStrategy::PostOutput() {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    if (!m_canOutput) {
        return;
    }
    
    if (!m_transmitSocket.IsWorking()) {
        return;
    }

    if (!socketBuffer.HasDataToSend()) {
        return;
    }

    const auto& transmitSockId = m_transmitSocket.GetSockId();
    while (true) {
        const auto pullLength = socketBuffer.PrepareSendPlan();
        if (pullLength < 1) {
            break;
        }

        const auto count = socketBuffer.SetupSendIov(m_sendOperation.m_iov);

        const auto sendLength = writev(transmitSockId, m_sendOperation.m_iov, count);
        if (sendLength < 0) {
            const auto error = SAGetLastError();
            if (IsInterrupted(error)) {
                continue;
            }

            if (IsWouldBlock(error)) {
                m_canOutput = false;
                break;
            }

            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            m_transmitSocket.Close();
            m_transmitSocket.OnError(error);
            break;
        }

        m_transmitSocket.OnSended(nullptr, sendLength);

        const auto restLength = socketBuffer.PreserveSendPlan(sendLength);
        MZ_UNUSED_VARIABLE(restLength);
    }
}

void EpollTransmitStrategy::EnableOutput() {
    m_canOutput = true;
}

DECL_NAMESPACE_MZ_NET_END

#endif
