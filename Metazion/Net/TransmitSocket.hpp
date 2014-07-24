#ifndef _MZ_NET_TRANSMITSOCKET_HPP_
#define _MZ_NET_TRANSMITSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket : public Socket {
    DISALLOW_COPY_AND_ASSIGN(TransmitSocket)

public:
    TransmitSocket();

    virtual ~TransmitSocket();

public:
    IoStrategy& GetIoStrategy() override final;

    void Reset() override;

    void Prepare() override;

    void Tick(int interval) override;

    void OnAttached() override;

    void OnStart() override;

    void OnClose() override;

    bool IsAlive() const override;

    virtual void OnConnected();

    virtual void OnDisconnected();

    virtual int OnSended(const void* data, int length);

    virtual int OnRecved(const void* data, int length);

    int Send(const void* data, int length);

    SocketBuffer& GetSocketBuffer();

protected:
    SocketBuffer m_socketBuffer;

private:
    TransmitStrategy m_transmitStrategy;
};

inline SocketBuffer& TransmitSocket::GetSocketBuffer() {
    return m_socketBuffer;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_TRANSMITSOCKET_HPP_
