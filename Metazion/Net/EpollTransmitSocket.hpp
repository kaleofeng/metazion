#ifndef MZ_NET_EPOLLTRANSMITSOCKET_HPP
#define MZ_NET_EPOLLTRANSMITSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/EpollSocket.hpp"
#include "Metazion/Net/TcpSocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class EpollTransmitSocket : public EpollSocket {
    DISALLOW_COPY_AND_ASSIGN(EpollTransmitSocket)

public:
    EpollTransmitSocket();

    virtual ~EpollTransmitSocket();

public: // @override
    void Reset();

    void OnStarted();

    bool IsClosing();

public:
    int Send(const void* data, int length);

    int SendNow(const void* data, int length);

private: // @override
    void Input();

    void Output();

protected:
    TcpSocketBuffer m_socketBuffer;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // MZ_NET_EPOLLTRANSMITSOCKET_HPP
