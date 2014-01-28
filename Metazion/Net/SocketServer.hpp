#ifndef _MZ_NET_SOCKETSERVER_HPP_
#define _MZ_NET_SOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class Socket;

class SocketServer {
    DISALLOW_COPY_AND_ASSIGN(SocketServer)

public:
    SocketServer();

    virtual ~SocketServer();

public:
    virtual bool Initialize(int socketCapacity, int ioThreadNumber) = 0;

    virtual void Finalize() = 0;

    virtual bool Attach(Socket* socket) = 0;

    virtual bool CanAttachMore() const = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKETSERVER_HPP_
