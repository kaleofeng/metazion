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
    auto ret = TransmitSocket::IsActive();
    if (!ret) {
        return false;
    }

    if (!IsStage(STAGE_CONNECTED)) {
        return false;
    }

    return true;
}

bool NormalClientSocket::IsAlive() const {
    auto ret = TransmitSocket::IsAlive();
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
    const auto now = NS_SHARE::GetTickMillisecond();
    if (now < m_connectTime) {
        return;
    }

    const auto ret = TryToConnect();
    if (ret == 0) {
        SetStage(STAGE_CONNECTING);
    }
    else if (ret > 0) {
        SetStage(STAGE_CONNECTED);
    }
    else {
        Reconnect(false);
    }
}

void NormalClientSocket::ConnectStageConnecting() {
    const auto ret = CheckConnected();
    if (ret == 0) {
        // Keep connecting.
    }
    else if (ret > 0) {
        SetStage(STAGE_CONNECTED);
    }
    else {
        DetachSockId();
        Reconnect(false);
    }
}

void NormalClientSocket::ConnectStageConnected() {
    if (!IsReady()) {
        Reconnect(true);
    }
}

void NormalClientSocket::ConnectStageClosed() {
    ASSERT_TRUE(!IsReady());
}

void NormalClientSocket::Reconnect(bool immediately) {
    if (m_reconnectInterval < 0) {
        SetStage(STAGE_CLOSED);
        return;
    }

    if (!immediately) {
        ResetConnectTime();
    }

    SetStage(STAGE_WAITING);
}

int NormalClientSocket::TryToConnect() {
    auto sockId = CreateSockId(TRANSPORT_TCP);
    if (INVALID_SOCKID == sockId) {
        return -1;
    }

    AttachSockId(sockId);

    auto sockAddr = m_remoteHost.SockAddr();
    auto sockAddrLen = m_remoteHost.SockAddrLen();
    const auto ret = ::connect(m_sockId, sockAddr, sockAddrLen);
    if (0 == ret) {
        return 1;
    }

    if (!IsWouldBlock()) {
        DetachSockId();
        return -1;
    }

    return 0;
}

int NormalClientSocket::CheckConnected() {
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(m_sockId, &wfds);

    fd_set efds;
    FD_ZERO(&efds);
    FD_SET(m_sockId, &efds);

    struct timeval timeout = { 0, 0 };
    const auto nfds = static_cast<int>(m_sockId + 1);
    const auto ret = ::select(nfds, nullptr, &wfds, &efds, &timeout);
    if (0 == ret) {
        return 0;
    }
    else if (ret < 0) {
        return -1;
    }

    if (FD_ISSET(m_sockId, &efds)) {
        return -1;
    }

    if (!FD_ISSET(m_sockId, &wfds)) {
        return -1;
    }

    // It doesn't work in some situations on linux platform.
    // For examle, when listen socket's backlog queue is full.
    // And also doesn't work on solaris platform.
    int optValue = 0;
    auto optLength = static_cast<SockLen_t>(sizeof(optValue));
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
