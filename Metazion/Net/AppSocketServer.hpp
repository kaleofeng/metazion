#ifndef _MZ_NET_APPSOCKETSERVER_HPP_
#define _MZ_NET_APPSOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Collection/StaticArray.hpp>

#include "Metazion/Net/NormalSocketServer.hpp"
#include "Metazion/Net/PacketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppSocketServer : public NormalSocketServer {
    DISALLOW_COPY_AND_ASSIGN(AppSocketServer)

public:
    using SocketArray_t = NS_SHARE::StaticArray<Socket*>;

public:
    struct SocketFilter {
        SocketFilter() {}
        virtual ~SocketFilter() {}

        virtual bool Filter(Socket* socket) { 
            return socket->GetType() != SOCKET_TCP_LISTEN;
        }
    };
    
public:
    AppSocketServer();

    virtual ~AppSocketServer();

public:
    bool Initialize(int socketCapacity, int ioThreadNumber) override;

    void Finalize() override;

    int LockSockets(SocketFilter& filter, SocketArray_t& socketArray);

    void UnlockSockets(SocketArray_t& socketArray);

    EncodeBuffer& GetEncodeBuffer();

    DecodeBuffer& GetDecodeBuffer();

private:
    int m_encodeBufferKey;
    int m_decodeBufferKey;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPSOCKETSERVER_HPP_
