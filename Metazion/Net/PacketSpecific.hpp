#ifndef _MZ_NET_PACKETSPECIFIC_HPP_
#define _MZ_NET_PACKETSPECIFIC_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Memory/StepBuffer.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>

#include "Metazion/Net/PacketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class PacketProcessor {
protected:
    PacketProcessor() {}

    virtual ~PacketProcessor() {}

public:
    virtual int PullPackets(void* buffer, int length, PacketArray_t& packetArray) = 0;
};

class PacketSpecific {
    DISALLOW_COPY_AND_ASSIGN(PacketSpecific)
        
public:
    PacketSpecific();
    
    ~PacketSpecific();
        
public:
    void Reset();

    void Rework();
    
    void SetUnpackBufferPool(UnpackBuffer::BufferPool_t& bufferPool);

    void SetPacketCachePool(PacketCache_t::BufferPool_t& bufferPool);

    bool Pack(int command, const void* data, int length, PackBuffer& packBuffer);
    
    bool Unpack(const void* data, int length);

    int PullPackets(void* buffer, int length, PacketArray_t& packetArray);

    int CheckPacketLength(const void* data, int length);
    
private:
    int PushPacket(const void* data, int length);

    int PullPacket(void* data, int length);
    
    int _PushPacket(const void* data, int length);

    int _PullPacket(void* data, int length);
    
private:
    NS_SHARE::MutexLock m_lock;
    UnpackBuffer m_unpackBuffer;
    UnpackBuffer::BufferPool_t* m_unpackBufferPool;
    PacketCache_t m_packetCache;
    PacketCache_t::BufferPool_t* m_packetCachePool;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_PACKETSPECIFIC_HPP_
