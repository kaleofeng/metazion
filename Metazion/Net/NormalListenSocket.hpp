#ifndef MZ_NET_NORMALLISTENSOCKET_HPP
#define MZ_NET_NORMALLISTENSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/ListenSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalServerSocket;

class NormalListenSocket : public ListenSocket {
    DISALLOW_COPY_AND_ASSIGN(NormalListenSocket)

public:
    NormalListenSocket();

    virtual ~NormalListenSocket();

public:
    int GetType() const override;

    bool OnAccepted(const SockId_t& sockId) override;

protected:
    virtual NormalServerSocket* CreateServerSocket() = 0;
};

inline int NormalListenSocket::GetType() const {
    return SOCKET_TCP_LISTEN;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_NORMALLISTENSOCKET_HPP
