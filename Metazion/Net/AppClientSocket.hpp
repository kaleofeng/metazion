#ifndef _MZ_NET_APPCLIENTSOCKET_HPP_
#define _MZ_NET_APPCLIENTSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/TransmitSocket.hpp"
#include "Metazion/Net/ComConnecter.hpp"
#include "Metazion/Net/ComPacketer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppClientSocket : public TransmitSocket {
    MZ_DISALLOW_COPY_AND_ASSIGN(AppClientSocket)

public:
    AppClientSocket();

    virtual ~AppClientSocket();

public:
    void Reset() override;

    void Tick(int interval) override;

    void Dispatch() override final;

public:
    ComConnecter m_connecter;
    ComPacketer m_packeter;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPCLIENTSOCKET_HPP_
