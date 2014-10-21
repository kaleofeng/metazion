#include "Metazion/Net/NormalClientSocket.hpp"

#include <Metazion/Share/Time/Time.hpp>

#include "Metazion/Net/NormalSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

NormalClientSocket::NormalClientSocket()
    : m_stage(STAGE_NONE)
    , m_connectTime(0)
    , m_reconnectInterval(0)
    , m_tempSockId(INVALID_SOCKID) {}

NormalClientSocket::~NormalClientSocket() {}

void NormalClientSocket::Reset() {
    TransmitSocket::Reset();
    m_remoteHost.Reset();
    m_stage = STAGE_NONE;
    m_connectTime = 0;
    m_reconnectInterval = 0;
    m_tempSockId = INVALID_SOCKID;
}

void NormalClientSocket::Tick(int interval) {
    TransmitSocket::Tick(interval);
    ConnectStage();
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

void NormalClientSocket::OnConnected() {}

void NormalClientSocket::OnDisconnected() {}

int NormalClientSocket::OnSended(const void* data, int length) {
    return length;
}

int NormalClientSocket::OnRecved(const void* data, int length) {
    return length;
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
    const auto now = NS_SHARE::GetNowMillisecond();
    if (now < m_connectTime) {
        return;
    }

    const auto ret = TryToConnect();
    if (ret == 0) {
        SetStage(STAGE_CONNECTING);
    }
    else if (ret > 0) {
        AttachSockId(m_tempSockId);
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
        AttachSockId(m_tempSockId);
        SetStage(STAGE_CONNECTED);
    }
    else {
        DetachTempSockId();
        Reconnect(false);
    }
}

void NormalClientSocket::ConnectStageConnected() {
    if (!IsActive()) {
        Reconnect(true);
    }
}

void NormalClientSocket::ConnectStageClosed() {
    ASSERT_TRUE(!IsActive());
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
    if (sockId == INVALID_SOCKID) {
        return -1;
    }

    AttachTempSockId(sockId);

    auto sockAddr = m_remoteHost.SockAddr();
    auto sockAddrLen = m_remoteHost.SockAddrLen();
    const auto ret = ::connect(m_tempSockId, sockAddr, sockAddrLen);
    if (ret < 0) {
        if (!IsWouldBlock()) {
            DetachTempSockId();
            return -1;
        }

        return 0;
    }

    return 1;
}

int NormalClientSocket::CheckConnected() {
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(m_tempSockId, &wfds);

    fd_set efds;
    FD_ZERO(&efds);
    FD_SET(m_tempSockId, &efds);

    struct timeval timeout = { 0, 0 };
    const auto nfds = static_cast<int>(m_tempSockId + 1);
    const auto ret = ::select(nfds, nullptr, &wfds, &efds, &timeout);
    if (ret == 0) {
        return 0;
    }
    else if (ret < 0) {
        return -1;
    }

    if (FD_ISSET(m_tempSockId, &efds)) {
        return -1;
    }

    if (!FD_ISSET(m_tempSockId, &wfds)) {
        return -1;
    }

    // It doesn't work in some situations on linux platform.
    // For examle, when listen socket's backlog queue is full.
    // And also doesn't work on solaris platform.
    int optValue = 0;
    auto optLength = static_cast<SockLen_t>(sizeof(optValue));
    GetSockOpt(m_tempSockId, SOL_SOCKET, SO_ERROR, &optValue, &optLength);
    if (optValue != 0) {
        return -1;
    }

    return 1;
}

void NormalClientSocket::ResetConnectTime() {
    m_connectTime = NS_SHARE::GetNowMillisecond() + m_reconnectInterval;
}

DECL_NAMESPACE_MZ_NET_END
