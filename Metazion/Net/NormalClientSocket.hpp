#ifndef _MZ_NET_NORMALCLIENTSOCKET_HPP_
#define _MZ_NET_NORMALCLIENTSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/TransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalClientSocket : public TransmitSocket {
    DISALLOW_COPY_AND_ASSIGN(NormalClientSocket)

    enum Stage {
        STAGE_NONE = 0,
        STAGE_WAITING,
        STAGE_CONNECTING,
        STAGE_CONNECTED,
        STAGE_CLOSED,
    };

public:
    NormalClientSocket();

    virtual ~NormalClientSocket();

public:
    int GetType() const override final;

    void Reset() override;

    void Tick(int interval) override;

    bool IsActive() const override;

    bool IsAlive() const override;

    void SetRemoteHost(const char* ip, int port);

    void SetReconnectInterval(int milliseconds);

    bool Connect();

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

protected:
    Host m_remoteHost;
    int m_stage;
    int32_t m_connectTime;
    int m_reconnectInterval;
};

inline int NormalClientSocket::GetType() const {
    return SOCKET_TCP_CLIENT;
}

inline bool NormalClientSocket::IsStage(int stage) const {
    return stage == m_stage;
}

inline void NormalClientSocket::SetStage(int stage) {
    m_stage = stage;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_NORMALCLIENTSOCKET_HPP_
