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

    void OnClose() override final;

    void OnError(int error) override final;

    int Send(const void* data, int length);

    SocketBuffer& GetSocketBuffer();

protected:
    virtual void OnConnected() = 0;

    virtual void OnDisconnected() = 0;

    virtual int OnSended(const void* data, int length) = 0;

    virtual int OnRecved(const void* data, int length) = 0;

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
