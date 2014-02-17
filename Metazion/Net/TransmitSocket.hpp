#ifndef _MZ_NET_TRANSMITSOCKET_HPP_
#define _MZ_NET_TRANSMITSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/TcpSocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket : public Socket {
    DISALLOW_COPY_AND_ASSIGN(TransmitSocket)

public:
    TransmitSocket();

    virtual ~TransmitSocket();

public:
    IoStrategy& GetIoStrategy() override;

    void Reset() override;

    void OnStarted() override;

    bool IsAlive() const override;

    int Send(const void* data, int length);

    TcpSocketBuffer& GetSocketBuffer();

protected:
    TcpSocketBuffer m_socketBuffer;

private:
    TransmitStrategy m_transmitStrategy;
};

inline IoStrategy& TransmitSocket::GetIoStrategy() {
    return m_transmitStrategy;
}

inline TcpSocketBuffer& TransmitSocket::GetSocketBuffer() {
    return m_socketBuffer;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_TRANSMITSOCKET_HPP_
