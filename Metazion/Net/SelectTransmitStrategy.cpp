#include "Metazion/Net/SelectTransmitStrategy.hpp"

#include <Metazion/Share/Log/Logger.hpp>

#include "Metazion/Net/TransmitSocket.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

SelectTransmitStrategy::SelectTransmitStrategy(TransmitSocket& transmitSocket)
    : m_transmitSocket(transmitSocket) {}

SelectTransmitStrategy::~SelectTransmitStrategy() {}

void SelectTransmitStrategy::Reset() {
    m_canOutput = false;
}

void SelectTransmitStrategy::Prepare() {
    Reset();
}

void SelectTransmitStrategy::Start() {
    // Nothing to do.
}

void SelectTransmitStrategy::Launch() {
    // Nothing to do.
}

bool SelectTransmitStrategy::IsBusy() const {
    return false;
}

void SelectTransmitStrategy::PostInput() {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    if (!m_canInput) {
        return;
    }

    const auto& transmitSockId = m_transmitSocket.GetSockId();
    while (true) {
        const auto pushLength = socketBuffer.PrepareRecvPlan();
        MZ_UNUSED_VARIABLE(pushLength);

        const auto count = socketBuffer.SetupRecvIov(m_recvOperation.m_iov);
        MZ_UNUSED_VARIABLE(count);

        auto buf = static_cast<char*>(m_recvOperation.m_iov[0].IOV_PTR_FIELD);
        auto len = static_cast<int>(m_recvOperation.m_iov[0].IOV_LEN_FIELD);
        const auto recvLength = recv(transmitSockId, buf, len, 0);
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
                m_canInput = false;
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
            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Trace: socket close. [%s:%d]\n", __FILE__, __LINE__);
            m_transmitSocket.Disconnect();
        }
    }
}

void SelectTransmitStrategy::PostOutput() {
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
        MZ_UNUSED_VARIABLE(count);

        auto buf = static_cast<char*>(m_sendOperation.m_iov[0].IOV_PTR_FIELD);
        auto len = static_cast<int>(m_sendOperation.m_iov[0].IOV_LEN_FIELD);
        const auto sendLength = send(transmitSockId, buf, len, 0);
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

void SelectTransmitStrategy::EnableInput() {
    m_canInput = true;
}

void SelectTransmitStrategy::EnableOutput() {
    m_canOutput = true;
}

DECL_NAMESPACE_MZ_NET_END

#endif
