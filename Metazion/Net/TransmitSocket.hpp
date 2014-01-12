#ifndef MZ_NET_TRANSMITSOCKET_HPP
#define MZ_NET_TRANSMITSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/TcpSocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket : public Socket {
    DISALLOW_COPY_AND_ASSIGN(TransmitSocket)

public:
    TransmitSocket();

    virtual ~TransmitSocket();

public: // @Override
    void Reset();

    void OnStarted();

    bool IsAlive() const;

public:
    int Send(const void* data, int length);

    TcpSocketBuffer& GetSocketBuffer() {
        return m_socketBuffer;
    }

    IoStrategy& GetIoStrategy() {
        return m_transmitStrategy;
    }

protected:
    TcpSocketBuffer m_socketBuffer;

private:
    TransmitStrategy m_transmitStrategy;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_TRANSMITSOCKET_HPP
