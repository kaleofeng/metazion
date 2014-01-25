#ifndef MZ_NET_NORMALSERVERSOCKET_HPP
#define MZ_NET_NORMALSERVERSOCKET_HPP

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

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_NORMALSERVERSOCKET_HPP
