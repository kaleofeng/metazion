#ifndef _MZ_NET_BASESOCKETSERVER_HPP_
#define _MZ_NET_BASESOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/EpollSocketServer.hpp"
#include "Metazion/Net/IocpSocketServer.hpp"
#include "Metazion/Net/SocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

#if defined(MZ_PLATFORM_WINOWS)
typedef IocpSocketServer IoSocketServer;
#endif // MZ_PLATFORM_WINOWS

#if defined(MZ_PLATFORM_LINUX)
typedef EpollSocketServer IoSocketServer;
#endif // MZ_PLATFORM_LINUX

class BaseSocketServer : public IoSocketServer {
    DISALLOW_COPY_AND_ASSIGN(BaseSocketServer)

public:
    BaseSocketServer();

    virtual ~BaseSocketServer();

public:
    SocketBuffer::SendCache_t::BufferPool_t& GetSendCachePool();

    SocketBuffer::RecvCache_t::BufferPool_t& GetRecvCachePool();

private:
    SocketBuffer::SendCache_t::BufferPool_t m_sendCachePool;
    SocketBuffer::RecvCache_t::BufferPool_t m_recvCachePool;
};

inline SocketBuffer::SendCache_t::BufferPool_t& BaseSocketServer::GetSendCachePool() {
    return m_sendCachePool;
}

inline SocketBuffer::RecvCache_t::BufferPool_t& BaseSocketServer::GetRecvCachePool() {
    return m_recvCachePool;
}


DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_BASESOCKETSERVER_HPP_
