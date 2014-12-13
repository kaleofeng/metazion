#ifndef _MZ_NET_GENERICNETWORKSERVICE_HPP_
#define _MZ_NET_GENERICNETWORKSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/NetworkService.hpp"
#include "Metazion/Net/SocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class GenericNetworkService : public NetworkService {
    MZ_DISALLOW_COPY_AND_ASSIGN(GenericNetworkService)

public:
    GenericNetworkService();

    virtual ~GenericNetworkService();

public:
    SocketBuffer::SendCache_t::BufferPool_t& GetSendCachePool();

    SocketBuffer::RecvCache_t::BufferPool_t& GetRecvCachePool();

private:
    SocketBuffer::SendCache_t::BufferPool_t m_sendCachePool;
    SocketBuffer::RecvCache_t::BufferPool_t m_recvCachePool;
};

inline SocketBuffer::SendCache_t::BufferPool_t& GenericNetworkService::GetSendCachePool() {
    return m_sendCachePool;
}

inline SocketBuffer::RecvCache_t::BufferPool_t& GenericNetworkService::GetRecvCachePool() {
    return m_recvCachePool;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_GENERICNETWORKSERVICE_HPP_
