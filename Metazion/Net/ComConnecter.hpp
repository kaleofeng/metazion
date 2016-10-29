#ifndef _MZ_NET_COMCONNECTER_HPP_
#define _MZ_NET_COMCONNECTER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <functional>

#include "Metazion/Net/Host.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class ComConnecter {
    MZ_DISALLOW_COPY_AND_ASSIGN(ComConnecter)

    enum Stage {
        STAGE_NONE = 0,
        STAGE_WAITING,
        STAGE_CONNECTING,
        STAGE_CONNECTED,
        STAGE_CLOSED,
    };

    using ConnectFailedCallback_t = std::function<void()>;

private:
    TransmitSocket& m_socket;

    int m_stage{ STAGE_NONE };
    int64_t m_connectTime{ 0 };
    int m_reconnectInterval{ 0 };
    SockId_t m_tempSockId{ INVALID_SOCKID };

    ConnectFailedCallback_t m_connectFaildCallback;

public:
    ComConnecter(TransmitSocket& socket);

    ~ComConnecter();

public:
    void Reset();

    void Tick(int interval);

    bool Connect();

    int GetReconnectInterval() const;

    void SetReconnectInterval(int milliseconds);

    void SetConnectFailedCallback(ConnectFailedCallback_t callback);

private:
    void ConnectStage();

    void ConnectStageWaiting();

    void ConnectStageConnecting();

    void ConnectStageConnected();

    void ConnectStageClosed();

    void Reconnect(bool immediately);

    int TryToConnect();

    int CheckConnected();

    void ResetConnectTime();

    bool IsStage(int stage) const;

    void SetStage(int stage);

    void AttachTempSockId(const SockId_t& sockId);

    void DetachTempSockId();
};

inline int ComConnecter::GetReconnectInterval() const {
    return m_reconnectInterval;
}

inline void ComConnecter::SetReconnectInterval(int milliseconds) {
    m_reconnectInterval = milliseconds;
}

inline void ComConnecter::SetConnectFailedCallback(ConnectFailedCallback_t callback) {
    m_connectFaildCallback = callback;
}

inline bool ComConnecter::IsStage(int stage) const {
    return m_stage == stage;
}

inline void ComConnecter::SetStage(int stage) {
    m_stage = stage;
}

inline void ComConnecter::AttachTempSockId(const SockId_t& sockId) {
    m_tempSockId = sockId;
}

inline void ComConnecter::DetachTempSockId() {
    if (m_tempSockId != INVALID_SOCKID) {
        DestroySockId(m_tempSockId);
        m_tempSockId = INVALID_SOCKID;
    }
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_COMCONNECTER_HPP_
