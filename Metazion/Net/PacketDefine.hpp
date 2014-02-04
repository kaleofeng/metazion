#ifndef _MZ_NET_PACKETDEFINE_HPP_
#define _MZ_NET_PACKETDEFINE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Collection/DynamicArray.hpp>
#include <Metazion/Share/Memory/ObjectPool.hpp>
#include <Metazion/Share/Memory/PieceBuffer.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

struct PacketHeader {
    enum {
        MULTIPLEBEGIN = -1,
        MULTIPLE = -2,
        MULTIPLEEND = -3,
    };

    PacketHeader()
        : m_command(0)
        , m_length(0) {}

    int16_t m_command;
    int16_t m_length;
};

enum PacketValue {
    MAXNETPACKETLENGTH = 1024,
    MAXNETDATALENGTH = MAXNETPACKETLENGTH - sizeof(PacketHeader),
    
    MAXAPPDATALENGTH = 16 * 1024,
    MAXAPPPACKETLENGTH = MAXAPPDATALENGTH + sizeof(PacketHeader),
};

struct PackBuffer {
    enum { LENGTH = MAXAPPPACKETLENGTH + 256 };

    using Buffer_t = NS_SHARE::PieceBuffer<LENGTH>;
    
    PackBuffer() {}
    
    Buffer_t m_tempBuffer;
    Buffer_t m_resultBuffer;
};

struct UnpackBuffer {
    enum { LENGTH = MAXAPPPACKETLENGTH + 256 };

    using Buffer_t = NS_SHARE::PieceBuffer<LENGTH>;
    using BufferPool_t = NS_SHARE::ObjectPool<Buffer_t
        , NS_SHARE::StepAllocator<256>, NS_SHARE::MutexLock>;

    UnpackBuffer()
        : m_buffer(nullptr)
        , m_bufferPool(nullptr) {}

    Buffer_t* m_buffer;
    BufferPool_t* m_bufferPool;
};

struct ThreadPackBuffer {
    enum { TAG = 0xFFEEDDCC };

    ThreadPackBuffer()
        : m_tag(TAG) {}

    uint32_t m_tag;
    PackBuffer m_packBuffer;
};

using PacketCache_t = NS_SHARE::StepBuffer<1024 * 4, 1024 * 256>;

using PacketArray_t = NS_SHARE::DynamicArray<void*>;

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_PACKETDEFINE_HPP_
