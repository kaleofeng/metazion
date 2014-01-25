#ifndef MZ_NET_NORMALSOCKETSERVER_HPP
#define MZ_NET_NORMALSOCKETSERVER_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/BaseSocketServer.hpp"
#include "Metazion/Net/TcpSocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalSocketServer : public BaseSocketServer {
    DISALLOW_COPY_AND_ASSIGN(NormalSocketServer)
    
public:
    NormalSocketServer();

    virtual ~NormalSocketServer();

public:
    TcpSocketBuffer::SendCache_t::BufferPool_t& GetSendCachePool();

private:
    TcpSocketBuffer::SendCache_t::BufferPool_t m_sendCachePool;
};

inline TcpSocketBuffer::SendCache_t::BufferPool_t& NormalSocketServer::GetSendCachePool() {
    return m_sendCachePool;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_NORMALSOCKETSERVER_HPP
