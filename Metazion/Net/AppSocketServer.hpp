#ifndef MZ_NET_APPSOCKETSERVER_HPP
#define MZ_NET_APPSOCKETSERVER_HPP

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Collection/DynamicArray.hpp>
#include "Metazion/Net/NormalSocketServer.hpp"
#include "Metazion/Net/PacketSpecific.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppSocketServer : public NormalSocketServer {
    DISALLOW_COPY_AND_ASSIGN(AppSocketServer)

public:
    using SocketArray_t = NS_SHARE::DynamicArray<Socket*>;

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

    PackBuffer& GetPackBuffer();

    UnpackBuffer::BufferPool_t& GetUnpackBufferPool();

    PacketCache_t::BufferPool_t& GetPacketCachePool();

private:
    int m_packBufferKey;
    UnpackBuffer::BufferPool_t m_unpackBufferPool;
    PacketCache_t::BufferPool_t m_packetCachePool;
};

inline UnpackBuffer::BufferPool_t& AppSocketServer::GetUnpackBufferPool() {
    return m_unpackBufferPool;
}

inline PacketCache_t::BufferPool_t& AppSocketServer::GetPacketCachePool() {
    return m_packetCachePool;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_APPSOCKETSERVER_HPP
