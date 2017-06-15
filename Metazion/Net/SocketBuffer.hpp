#ifndef _MZ_NET_TCPSOCKETBUFFER_HPP_
#define _MZ_NET_TCPSOCKETBUFFER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <mutex>

#include <Metazion/Share/Memory/InputBuffer.hpp>
#include <Metazion/Share/Memory/OutputBuffer.hpp>

#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class SocketBuffer {
    MZ_DISALLOW_COPY_AND_ASSIGN(SocketBuffer)

    using Lock_t = std::mutex;
    using LockGuard_t = std::lock_guard<Lock_t>;

public:
    using SendCache_t = NS_SHARE::InputBuffer<SINGLE_BUFFER_LENGTH, SINGLE_BUFFER_SIZE>;
    using RecvCache_t = NS_SHARE::OutputBuffer<SINGLE_BUFFER_LENGTH, SINGLE_BUFFER_SIZE>;
    using SendPlan_t = NS_SHARE::OutputBuffer<SINGLE_BUFFER_LENGTH, NUMBER_SEND_IOV>;
    using RecvPlan_t = NS_SHARE::InputBuffer<SINGLE_BUFFER_LENGTH, NUMBER_RECV_IOV>;

public:
    std::mutex m_sendLock;
    std::mutex m_recvLock;

    SendCache_t::BufferPool_t* m_sendCachePool;
    RecvCache_t::BufferPool_t* m_recvCachePool;

    SendCache_t m_sendCache;
    RecvCache_t m_recvCache;
    
    SendPlan_t m_sendPlan;
    RecvPlan_t m_recvPlan;

public:
    SocketBuffer();
    
    ~SocketBuffer();

    void Reset();

    void Prepare();

    void SetSendBufferPool(SendCache_t::BufferPool_t& bufferPool);

    void SetRecvCachePool(SendCache_t::BufferPool_t& bufferPool);

    int SetupSendIov(IOV_TYPE iovs[NUMBER_SEND_IOV]);

    int SetupRecvIov(IOV_TYPE iovs[NUMBER_RECV_IOV]);

    int PrepareSendPlan();

    int PrepareRecvPlan();

    int PreserveSendPlan(int length);

    int PreserveRecvPlan(int length);

    bool HasDataToSend() const;

    bool HasDataRecvedYet() const;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_TCPSOCKETBUFFER_HPP_
