#include "Metazion/Net/EpollTransmitSocket.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_NET_BEGIN

EpollTransmitSocket::EpollTransmitSocket()
: m_socketBuffer(m_lock) {}

EpollTransmitSocket::~EpollTransmitSocket() {}

void EpollTransmitSocket::Reset() {
    EpollSocket::Reset();
    m_socketBuffer.Reset();
}

void EpollTransmitSocket::OnStarted() {
    EpollSocket::OnStarted();
    m_socketBuffer.Rework();
}

bool EpollTransmitSocket::IsClosing() {
    bool ret = EpollSocket::IsClosing();
    if (ret) {
        return true;
    }

    return false;
}

int EpollTransmitSocket::Send(const void* data, int length) {
    if (!IsIoAvailable()) {
        return 0;
    }

    m_lock.Lock();
    const int pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_lock.Unlock();

    if (pushLength < length) {
        return 0;
    }

    return pushLength;
}

int EpollTransmitSocket::SendNow(const void* data, int length) {
    if (!IsIoAvailable()) {
        return 0;
    }

    m_lock.Lock();
    const int pushLength = m_socketBuffer.m_sendCache.Push(data, length);
    m_lock.Unlock();

    if (pushLength < length) {
        return 0;
    }

    return pushLength;
}

void EpollTransmitSocket::Input() {
    while (true) {
        char* pushBuffer = m_socketBuffer.m_recvBuffer.GetPushBuffer();
        const int pushLength = m_socketBuffer.m_recvBuffer.GetPushLength();

        const int recvLength = ::recv(m_sockId, pushBuffer, pushLength, 0);
        if (0 == recvLength) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            Close();
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
            Close();
            OnError(error);
            break;
        }

        m_socketBuffer.m_recvBuffer.JumpPushIndex(recvLength);

        const char* pullBuffer = m_socketBuffer.m_recvBuffer.GetPullBuffer();
        const int pullLength = m_socketBuffer.m_recvBuffer.GetPullLength();

        const int processLength = OnRecvData(pullBuffer, pullLength);
        if (processLength < 0) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            Close();
            break;
        }
    
        m_socketBuffer.m_recvBuffer.JumpPullIndex(processLength);
        m_socketBuffer.m_recvBuffer.Compact();
    }
}

void EpollTransmitSocket::Output() {
    if (!IsReady()) {
        return;
    }

    if (!IsCanOutput()) {
        return;
    }

    if (!m_socketBuffer.HasDataToSend()) {
        return;
    }

    while (true) {
        int pullLength = m_socketBuffer.m_sendBuffer.GetPullLength();
        if (pullLength <= 0) {
            pullLength = m_socketBuffer.PrepareSendBuffer();    
        }
        if (pullLength <= 0) {
            break;
        }

        char* pullBuffer = m_socketBuffer.m_sendBuffer.GetPullBuffer();

        const int sendLength = ::send(m_sockId, pullBuffer, pullLength, MSG_NOSIGNAL);
        if (sendLength < 0) {
            const int error = GetLastError();
            if (EINTR == error) {
                continue;
            }
            if (EAGAIN == error || EWOULDBLOCK == error) {
                SetCanOutput(false);
                break;
            }

            ::printf("Socket Error: socket close, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
            Close();
            OnError(error);
            break;
        }

        const int processLength = OnSendData(pullBuffer, sendLength);
        if (processLength < 0) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            Close();
            break;
        }

        m_socketBuffer.m_sendBuffer.JumpPullIndex(processLength);
        m_socketBuffer.m_sendBuffer.Compact();
    }
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
