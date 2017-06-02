#include "Metazion/Net/ComConnecter.hpp"

#include <Metazion/Share/Time/Time.hpp>

#include "Metazion/Net/TransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

ComConnecter::ComConnecter(TransmitSocket& socket)
    : m_socket(socket) {}

ComConnecter::~ComConnecter() {}

void ComConnecter::Reset() {
    m_stage = STAGE_NONE;
    m_connectTime = 0;
    m_reconnectInterval = 0;
    m_tempSockId = INVALID_SOCKID;
}

void ComConnecter::Tick(int interval) {
    ConnectStage();
}

bool ComConnecter::Connect() {
    SetStage(STAGE_WAITING);
    return true;
}

void ComConnecter::ConnectStage() {
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
    default: MZ_ASSERT_TRUE(false); break;
    }
}

void ComConnecter::ConnectStageWaiting() {
    const auto now = NS_SHARE::GetNowMillisecond();
    if (now < m_connectTime) {
        return;
    }

    const auto ret = TryToConnect();
    if (ret == 0) {
        SetStage(STAGE_CONNECTING);
    }
    else if (ret > 0) {
        m_socket.AttachSockId(m_tempSockId);
        SetStage(STAGE_CONNECTED);
    }
    else {
        m_connectFaildCallback();
        Reconnect(m_reconnectInterval);
    }
}

void ComConnecter::ConnectStageConnecting() {
    const auto ret = CheckConnected();
    if (ret == 0) {
        // Keep connecting.
    }
    else if (ret > 0) {
        m_socket.AttachSockId(m_tempSockId);
        SetStage(STAGE_CONNECTED);
    }
    else {
        DetachTempSockId();
        m_connectFaildCallback();
        Reconnect(m_reconnectInterval);
    }
}

void ComConnecter::ConnectStageConnected() {
    if (m_socket.IsActive()) {
        return;
    }

    if (m_socket.IsWannaClose()) {
        return;
    }

    Reconnect(1000);
}

void ComConnecter::ConnectStageClosed() {
    MZ_ASSERT_TRUE(!m_socket.IsActive());
}

void ComConnecter::Reconnect(int milliseconds) {
    if (m_reconnectInterval < 0) {
        SetStage(STAGE_CLOSED);
        return;
    }

    ResetConnectTime(milliseconds);
    SetStage(STAGE_WAITING);
}

int ComConnecter::TryToConnect() {
    auto sockId = CreateSockId(TRANSPORT_TCP);
    if (sockId == INVALID_SOCKID) {
        return -1;
    }

    AttachTempSockId(sockId);

    auto sockAddr = m_socket.GetRemoteHost().SockAddr();
    auto sockAddrLen = m_socket.GetRemoteHost().SockAddrLen();
    const auto ret = connect(m_tempSockId, sockAddr, sockAddrLen);
    if (ret < 0) {
        const auto error = SAGetLastError();
        if (!IsConnectWouldBlock(error)) {
            DetachTempSockId();
            return -1;
        }

        return 0;
    }

    return 1;
}

int ComConnecter::CheckConnected() {
    const auto ret = CheckSockConnected(m_tempSockId);
    if (ret <= 0) {
        return ret;
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

void ComConnecter::ResetConnectTime(int milliseconds) {
    m_connectTime = NS_SHARE::GetNowMillisecond() + milliseconds;
}

DECL_NAMESPACE_MZ_NET_END
