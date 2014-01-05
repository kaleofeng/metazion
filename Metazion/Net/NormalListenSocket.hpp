#ifndef MZ_NET_NORMALLISTENSOCKET_HPP
#define MZ_NET_NORMALLISTENSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/BaseListenSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalServerSocket;

class NormalListenSocket : public BaseListenSocket {
    DISALLOW_COPY_AND_ASSIGN(NormalListenSocket)

public:
    NormalListenSocket();

    virtual ~NormalListenSocket();

public: // @Override
    int GetType() const;

    bool OnAccepted(const SockId_t& sockId);

protected:
    virtual NormalServerSocket* CreateServerSocket() = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_NORMALLISTENSOCKET_HPP
