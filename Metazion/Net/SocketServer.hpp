#ifndef MZ_NET_SOCKETSERVER_HPP
#define MZ_NET_SOCKETSERVER_HPP

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

#endif // MZ_NET_SOCKETSERVER_HPP
