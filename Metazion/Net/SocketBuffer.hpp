#ifndef _MZ_NET_TCPSOCKETBUFFER_HPP_
#define _MZ_NET_TCPSOCKETBUFFER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/StepBuffer.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

class SocketBuffer {
    DISALLOW_COPY_AND_ASSIGN(SocketBuffer)
    
public:
    using SendCache_t = NS_SHARE::StepBuffer<1024 * 4, 1024 * 256>;
    using RecvCache_t = NS_SHARE::StepBuffer<1024 * 4, 1024 * 256>;
    using SendBuffer_t = NS_SHARE::PieceBuffer<4096>;
    using RecvBuffer_t = NS_SHARE::PieceBuffer<4096>;

public:
    SocketBuffer();
    
    ~SocketBuffer();

public:
    void SetSendCachePool(SendCache_t::BufferPool_t& bufferPool);

    void SetRecvCachePool(SendCache_t::BufferPool_t& bufferPool);

    void Reset();

    void Rework();

    int PrepareSendBuffer();

    int PreserveRecvBuffer();

    bool HasDataToSend() const;

public:
    NS_SHARE::MutexLock m_sendLock;
    NS_SHARE::MutexLock m_recvLock;
    SendCache_t m_sendCache;
    SendCache_t::BufferPool_t* m_sendCachePool;
    RecvCache_t m_recvCache;
    RecvCache_t::BufferPool_t* m_recvCachePool;
    SendBuffer_t m_sendBuffer;
    RecvBuffer_t m_recvBuffer;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_TCPSOCKETBUFFER_HPP_
