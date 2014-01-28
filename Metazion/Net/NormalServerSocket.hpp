#ifndef _MZ_NET_NORMALSERVERSOCKET_HPP_
#define _MZ_NET_NORMALSERVERSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/TransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalServerSocket : public TransmitSocket {
    DISALLOW_COPY_AND_ASSIGN(NormalServerSocket)

public:
    NormalServerSocket();

    virtual ~NormalServerSocket();

public:
    int GetType() const override;

    void OnAttached() override;

    void OnStarted() override;

    void OnClosed() override;

    virtual void OnConnected();

    virtual void OnDisconnected();

    void Attach(const SockId_t& sockId);
};

inline int NormalServerSocket::GetType() const {
    return SOCKET_TCP_SERVER;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_NORMALSERVERSOCKET_HPP_
