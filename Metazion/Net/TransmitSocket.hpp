#ifndef _MZ_NET_TRANSMITSOCKET_HPP_
#define _MZ_NET_TRANSMITSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket : public Socket {
    MZ_DISALLOW_COPY_AND_ASSIGN(TransmitSocket)

    friend class TransmitStrategy;

public:
    TransmitSocket();

    virtual ~TransmitSocket();

public:
    void Reset() override;

    void Prepare() override;

    IoStrategy& GetIoStrategy() override final;

    bool IsAlive() const override final;

    void OnAttached() override final;

    void OnDetached() override final;

    void OnStart() override final;

    void OnStop() override final;

    void OnError(int error) override final;

    int Send(const void* data, int length);

    SocketBuffer& GetSocketBuffer();

protected:
    virtual void OnConnected();

    virtual void OnDisconnected();

    virtual void OnSended(const void* data, int length);

    virtual void OnRecved(const void* data, int length);

protected:
    TransmitStrategy m_transmitStrategy;
    SocketBuffer m_socketBuffer;

#if defined(MZ_ENABLE_STATISTIC)
    int64_t m_connectedTime;
    int64_t m_disconnectedTime;
    int64_t m_firstSendTime;
    int64_t m_lastSendTime;
    int64_t m_sendedBytes;
    int64_t m_firstRecvTime;
    int64_t m_lastRecvTime;
    int64_t m_recvedBytes;
#endif
};

inline IoStrategy& TransmitSocket::GetIoStrategy() {
    return m_transmitStrategy;
}

inline SocketBuffer& TransmitSocket::GetSocketBuffer() {
    return m_socketBuffer;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_TRANSMITSOCKET_HPP_
