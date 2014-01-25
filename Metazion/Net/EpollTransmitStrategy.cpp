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
    const SockId_t& transmitSockId = m_transmitSocket.GetSockId();
    while (true) {
        char* pushBuffer = m_transmitSocket.GetSocketBuffer().m_recvBuffer.GetPushBuffer();
        const int pushLength = m_transmitSocket.GetSocketBuffer().m_recvBuffer.GetPushLength();

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

        m_transmitSocket.GetSocketBuffer().m_recvBuffer.JumpPushIndex(recvLength);

        const char* pullBuffer = m_transmitSocket.GetSocketBuffer().m_recvBuffer.GetPullBuffer();
        const int pullLength = m_transmitSocket.GetSocketBuffer().m_recvBuffer.GetPullLength();

        const int processLength = m_transmitSocket.OnRecvData(pullBuffer, pullLength);
        if (processLength < 0) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            m_transmitSocket.Close();
            break;
        }
    
        m_transmitSocket.GetSocketBuffer().m_recvBuffer.JumpPullIndex(processLength);
        m_transmitSocket.GetSocketBuffer().m_recvBuffer.Compact();
    }
}

void EpollTransmitStrategy::Output() {
    if (!m_canOutput) {
        return;
    }
    
    if (!m_transmitSocket.IsReady()) {
        return;
    }

    if (!m_transmitSocket.GetSocketBuffer().HasDataToSend()) {
        return;
    }

    const SockId_t& transmitSockId = m_transmitSocket.GetSockId();
    while (true) {
        int pullLength = m_transmitSocket.GetSocketBuffer().m_sendBuffer.GetPullLength();
        if (pullLength <= 0) {
            pullLength = m_transmitSocket.GetSocketBuffer().PrepareSendBuffer();    
        }
        if (pullLength <= 0) {
            break;
        }

        char* pullBuffer = m_transmitSocket.GetSocketBuffer().m_sendBuffer.GetPullBuffer();

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

        const int processLength = m_transmitSocket.OnSendData(pullBuffer, sendLength);
        if (processLength < 0) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            m_transmitSocket.Close();
            break;
        }

        m_transmitSocket.GetSocketBuffer().m_sendBuffer.JumpPullIndex(processLength);
        m_transmitSocket.GetSocketBuffer().m_sendBuffer.Compact();
    }
}

void EpollTransmitStrategy::EnableOutput() {
    m_canOutput = true;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
