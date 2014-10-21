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
    int GetType() const override final;

    void Attach(const SockId_t& sockId);

protected:
    void OnConnected() override;

    void OnDisconnected() override;

    int OnSended(const void* data, int length) override;

    int OnRecved(const void* data, int length) override;
};

inline int NormalServerSocket::GetType() const {
    return SOCKET_TCP_SERVER;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_NORMALSERVERSOCKET_HPP_
