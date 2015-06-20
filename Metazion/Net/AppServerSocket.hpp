#ifndef _MZ_NET_APPSERVERSOCKET_HPP_
#define _MZ_NET_APPSERVERSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/TransmitSocket.hpp"
#include "Metazion/Net/ComPacketer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppServerSocket : public TransmitSocket {
    MZ_DISALLOW_COPY_AND_ASSIGN(AppServerSocket)

public:
    AppServerSocket();

    virtual ~AppServerSocket();

public:
    void Reset() override final;

    void Prepare() override final;

    void Tick(int interval) override final;

    void Dispatch() override final;

    int GetType() const override final;

    void Attach(const SockId_t& sockId);

protected:
    void OnConnected() override final;

    void OnDisconnected() override final;

    void OnSended(const void* data, int length) override final;

    void OnRecved(const void* data, int length) override final;

    virtual void DerivedReset() {};

    virtual void DerivedPrepare() {};

    virtual void DerivedTick(int interval) {};

    virtual void DerivedDispatch() {};

    virtual void DerivedOnConnected() {};

    virtual void DerivedOnDisconnected() {};

    virtual void DerivedOnSended(const void* data, int length) {};

    virtual void DerivedOnRecved(const void* data, int length) {};
    
public:
    ComPacketer m_packeter;
};

inline int AppServerSocket::GetType() const {
    return SOCKET_TCP_SERVER;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPSERVERSOCKET_HPP_
