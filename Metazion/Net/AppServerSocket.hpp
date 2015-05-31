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
    void Reset() override;

    void Dispatch() override final;

    void Attach(const SockId_t& sockId);
    
public:
    ComPacketer m_packeter;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPSERVERSOCKET_HPP_
