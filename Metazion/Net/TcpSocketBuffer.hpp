#ifndef _MZ_NET_TCPSOCKETBUFFER_HPP_
#define _MZ_NET_TCPSOCKETBUFFER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/StepBuffer.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

class TcpSocketBuffer {
    DISALLOW_COPY_AND_ASSIGN(TcpSocketBuffer)
    
public:
    using SendCache_t = NS_SHARE::StepBuffer<1024 * 4, 1024 * 256>;
    using SendBuffer_t = NS_SHARE::PieceBuffer<4096>;
    using RecvBuffer_t = NS_SHARE::PieceBuffer<4096>;

public:
    TcpSocketBuffer(NS_SHARE::MutexLock& lock);
    
    ~TcpSocketBuffer();

public:
    void SetSendCachePool(SendCache_t::BufferPool_t& bufferPool);

    void Reset();

    void Rework();

    bool HasDataToSend() const;

    int PrepareSendBuffer();

public:
    SendCache_t m_sendCache;
    SendCache_t::BufferPool_t* m_sendCachePool;
    SendBuffer_t m_sendBuffer;
    RecvBuffer_t m_recvBuffer;
    
private:
    NS_SHARE::MutexLock& m_lock;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_TCPSOCKETBUFFER_HPP_
