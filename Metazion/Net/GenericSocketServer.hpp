#ifndef _MZ_NET_GENERICSOCKETSERVER_HPP_
#define _MZ_NET_GENERICSOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/SocketServer.hpp"
#include "Metazion/Net/SocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class GenericSocketServer : public SocketServer {
    DISALLOW_COPY_AND_ASSIGN(GenericSocketServer)

public:
    GenericSocketServer();

    virtual ~GenericSocketServer();

public:
    SocketBuffer::SendCache_t::BufferPool_t& GetSendCachePool();

    SocketBuffer::RecvCache_t::BufferPool_t& GetRecvCachePool();

private:
    SocketBuffer::SendCache_t::BufferPool_t m_sendCachePool;
    SocketBuffer::RecvCache_t::BufferPool_t m_recvCachePool;
};

inline SocketBuffer::SendCache_t::BufferPool_t& GenericSocketServer::GetSendCachePool() {
    return m_sendCachePool;
}

inline SocketBuffer::RecvCache_t::BufferPool_t& GenericSocketServer::GetRecvCachePool() {
    return m_recvCachePool;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_GENERICSOCKETSERVER_HPP_
