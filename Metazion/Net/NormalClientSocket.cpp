#include "Metazion/Net/NormalClientSocket.hpp"

#include <Metazion/Share/Time/Time.hpp>

#include "Metazion/Net/NormalSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

NormalClientSocket::NormalClientSocket()
    : m_stage(STAGE_NONE)
    , m_connectTime(0)
    , m_reconnectInterval(0) {}

NormalClientSocket::~NormalClientSocket() {}

void NormalClientSocket::Reset() {
    TransmitSocket::Reset();
    m_remoteHost.Reset();
    m_stage = STAGE_NONE;
    m_connectTime = 0;
    m_reconnectInterval = 0;
}

void NormalClientSocket::Tick(int interval) {
    TransmitSocket::Tick(interval);
    ConnectStage();
}

bool NormalClientSocket::IsActive() const {
    bool ret = TransmitSocket::IsActive();
    if (!ret) {
        return false;
    }

    if (!IsStage(STAGE_CONNECTED)) {
        return false;
    }

    return true;
}

bool NormalClientSocket::IsAlive() const {
    bool ret = TransmitSocket::IsAlive();
    if (ret) {
        return true;
    }

    if (!IsStage(STAGE_CLOSED)) {
        return true;
    }

    return false;
}

void NormalClientSocket::SetRemoteHost(const char* ip, int port) {
    m_remoteHost.SetFamily(AF_INET);
    m_remoteHost.SetIp(ip);
    m_remoteHost.SetPort(port);
}

void NormalClientSocket::SetReconnectInterval(int milliseconds) {
    m_reconnectInterval = milliseconds;
}

bool NormalClientSocket::Connect() {
    SetStage(STAGE_WAITING);
    return true;
}

bool NormalClientSocket::Reconnect() {
    SockId_t sockId = CreateSockId(TRANSPORT_TCP);
    if (INVALID_SOCKID == sockId) {
        return false;
    }

    AttachSockId(sockId);

    SockAddr_t* sockAddr = m_remoteHost.SockAddr();
    SockLen_t sockAddrLen = m_remoteHost.SockAddrLen();
    const int ret = ::connect(m_sockId, sockAddr, sockAddrLen);
    if (0 == ret) {
        SetStage(STAGE_CONNECTED);
        return true;
    }

    if (IsWouldBlock()) {
        SetStage(STAGE_CONNECTING);
        return true;
    }

    DetachSockId();
    return false;
}

void NormalClientSocket::ConnectStage() {
    switch (m_stage) {
    case STAGE_WAITING:
        ConnectStageWaiting();
        break;
    case STAGE_CONNECTING:
        ConnectStageConnecting();
        break;
    case STAGE_CONNECTED:
        ConnectStageConnected();
        break;
    case STAGE_CLOSED:
        ConnectStageClosed();
        break;
    default: ASSERT_TRUE(false); break;
    }
}

void NormalClientSocket::ConnectStageWaiting() {
    if (m_reconnectInterval <= 0) {
        SetStage(STAGE_CLOSED);
        return;
    }

    const int32_t now = NS_SHARE::GetTickMillisecond();
    if (now < m_connectTime) {
        return;
    }

    Reconnect();
    ResetConnectTime();

    SetStage(STAGE_CONNECTING);
}

void NormalClientSocket::ConnectStageConnecting() {
    const int ret = CheckConnected();
    if (ret > 0) {
        SetStage(STAGE_CONNECTED);
        return;
    }
    
    if (ret < 0) {
        SetStage(STAGE_WAITING);
        DetachSockId();
        ResetConnectTime();
        return;
    }

    // Keep connecting.
}

void NormalClientSocket::ConnectStageConnected() {
    if (!IsReady()) {
        SetStage(STAGE_WAITING);
    }
}

void NormalClientSocket::ConnectStageClosed() {
    ASSERT_TRUE(!IsReady());
}

int NormalClientSocket::CheckConnected() {
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(m_sockId, &wfds);

    fd_set efds;
    FD_ZERO(&efds);
    FD_SET(m_sockId, &efds);

    struct timeval timeout = { 0, 0 };
    const int nfds = static_cast<int>(m_sockId + 1);
    const int ret = ::select(nfds, nullptr, &wfds, &efds, &timeout);
    if (0 == ret) {
        return 0;
    }

    if (ret < 0) {
        return -1;
    }

    if (FD_ISSET(m_sockId, &efds)) {
        return -1;
    }

    if (!FD_ISSET(m_sockId, &wfds)) {
        return -1;
    }

    // It doesn't work in some situations in linux platform.
    // For examle, when listen socket's backlog queue is full.
    int optValue = 0;
    SockLen_t optLength = sizeof(optValue);
    GetSockOpt(m_sockId, SOL_SOCKET, SO_ERROR, &optValue, &optLength);
    if (0 != optValue) {
        return -1;
    }

    return 1;
}

void NormalClientSocket::ResetConnectTime() {
    m_connectTime = NS_SHARE::GetTickMillisecond() + m_reconnectInterval;
}

DECL_NAMESPACE_MZ_NET_END
