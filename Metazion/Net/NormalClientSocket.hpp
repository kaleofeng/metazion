#ifndef MZ_NET_NORMALCLIENTSOCKET_HPP
#define MZ_NET_NORMALCLIENTSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/BaseTransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalClientSocket : public BaseTransmitSocket {
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

public: // @override
    int GetType() const;

    void Reset();

    void OnAttached();

    void OnStarted();

    void OnClosed();

    void Tick(int interval);

    bool IsActive();

    bool IsClosing();

    virtual void OnConnected();

    virtual void OnDisconnected();

public:
    bool Connect(const char* ip, int port);

    void SetReconnectInterval(int milliseconds);

private:
    bool Reconnect();

    void ConnectStage();

    void ConnectStageWaiting();

    void ConnectStageConnecting();

    void ConnectStageConnected();

    void ConnectStageClosed();

    int CheckConnected();

    void ResetConnectTime();

    bool IsStage(int stage) const {
        return stage == m_stage;
    }

    void SetStage(int stage) {
        m_stage = stage;
    }

protected:
    Host m_remoteHost;
    int m_stage;
    int32_t m_connectTime;
    int m_reconnectInterval;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_NORMALCLIENTSOCKET_HPP
