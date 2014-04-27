#include "Metazion/Net/EpollTransmitStrategy.hpp"

#include "Metazion/Net/TransmitSocket.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_NET_BEGIN

EpollTransmitStrategy::EpollTransmitStrategy(TransmitSocket& transmitSocket)
    : m_transmitSocket(transmitSocket)
    , m_canOutput(false) {}

EpollTransmitStrategy::~EpollTransmitStrategy() {}

void EpollTransmitStrategy::Reset() {
    m_canOutput = false;
}

bool EpollTransmitStrategy::IsBusy() const {
    return false;
}

void EpollTransmitStrategy::Input() {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    const SockId_t& transmitSockId = m_transmitSocket.GetSockId();
    while (true) {
        char* pushBuffer = socketBuffer.m_recvBuffer.GetPushBuffer();
        const int pushLength = socketBuffer.m_recvBuffer.GetPushLength();

        const int recvLength = ::recv(transmitSockId, pushBuffer, pushLength, 0);
        if (0 == recvLength) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            m_transmitSocket.Close();
            break;
        }
        else if (recvLength < 0) {
            const int error = GetLastError();
            if (EINTR == error) {
                continue;
            }
            if (EAGAIN == error || EWOULDBLOCK == error) {
                break;
            }

            ::printf("Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            m_transmitSocket.Close();
            m_transmitSocket.OnError(error);
            break;
        }

        socketBuffer.m_recvBuffer.JumpPushIndex(recvLength);

        const char* pullBuffer = socketBuffer.m_recvBuffer.GetPullBuffer();
        const int pullLength = socketBuffer.m_recvBuffer.GetPullLength();

        m_transmitSocket.OnRecved(pullBuffer, pullLength);

        const int processLength = socketBuffer.PreserveRecvBuffer();
        if (processLength < pullLength) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            m_transmitSocket.Close();
        }
    }
}

void EpollTransmitStrategy::Output() {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    if (!m_canOutput) {
        return;
    }
    
    if (!m_transmitSocket.IsReady()) {
        return;
    }

    if (!socketBuffer.HasDataToSend()) {
        return;
    }

    const auto& transmitSockId = m_transmitSocket.GetSockId();
    while (true) {
        int pullLength = socketBuffer.m_sendBuffer.GetPullLength();
        if (pullLength <= 0) {
            pullLength = socketBuffer.PrepareSendBuffer();    
        }
        if (pullLength <= 0) {
            break;
        }

        const char* pullBuffer = socketBuffer.m_sendBuffer.GetPullBuffer();

        const int sendLength = ::send(transmitSockId, pullBuffer, pullLength, MSG_NOSIGNAL);
        if (sendLength < 0) {
            const int error = GetLastError();
            if (EINTR == error) {
                continue;
            }
            if (EAGAIN == error || EWOULDBLOCK == error) {
                m_canOutput = false;
                break;
            }

            ::printf("Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            m_transmitSocket.Close();
            m_transmitSocket.OnError(error);
            break;
        }

        m_transmitSocket.OnSended(pullBuffer, sendLength);

        socketBuffer.m_sendBuffer.JumpPullIndex(sendLength);
        socketBuffer.m_sendBuffer.Compact();
    }
}

void EpollTransmitStrategy::EnableOutput() {
    m_canOutput = true;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
