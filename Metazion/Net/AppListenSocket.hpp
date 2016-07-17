#ifndef _MZ_NET_APPLISTENSOCKET_HPP_
#define _MZ_NET_APPLISTENSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/ListenSocket.hpp"
#include "Metazion/Net/ComAccepter.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppListenSocket : public ListenSocket {
    MZ_DISALLOW_COPY_AND_ASSIGN(AppListenSocket)

public:
    ComAccepter m_accepter;

public:
    AppListenSocket();

    virtual ~AppListenSocket();

public:
    void Reset() override final;

    void Prepare() override final;

    void Tick(int interval) override final;

    void Dispatch() override final;

    int GetType() const override final;

protected:
    void OnWatched() override final;

    void OnUnwatched() override final;

    bool OnAccepted(const SockId_t& sockId, const Host& host) override final;

    virtual void DerivedReset() {};

    virtual void DerivedPrepare() {};

    virtual void DerivedTick(int interval) {};

    virtual void DerivedDispatch() {};

    virtual void DerivedOnWatched() {}

    virtual void DerivedOnUnwatched() {}

    virtual bool DerivedOnAccepted(const SockId_t& sockId, const Host& host) { return true; }
};

inline int AppListenSocket::GetType() const {
    return SOCKET_TCP_LISTEN;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPLISTENSOCKET_HPP_
