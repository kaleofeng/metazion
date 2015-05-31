#ifndef _MZ_NET_APPLISTENSOCKET_HPP_
#define _MZ_NET_APPLISTENSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/ListenSocket.hpp"
#include "Metazion/Net/ComAccepter.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppListenSocket : public ListenSocket {
    MZ_DISALLOW_COPY_AND_ASSIGN(AppListenSocket)

public:
    AppListenSocket();

    virtual ~AppListenSocket();

public:
    void Reset() override;

    void Dispatch() override final;

protected:
    bool OnAccepted(const SockId_t& sockId) override final;

public:
    ComAccepter m_accepter;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPLISTENSOCKET_HPP_
