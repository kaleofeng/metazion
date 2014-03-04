#ifndef _MZ_NET_PACKETDEFINE_HPP_
#define _MZ_NET_PACKETDEFINE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Collection/StaticArray.hpp>
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
    MAXNETDATALENGTH = 1024,
    MAXNETPACKETLENGTH = MAXNETDATALENGTH + sizeof(PacketHeader),
    
    MAXAPPDATALENGTH = 16 * 1024,
    MAXAPPPACKETLENGTH = MAXAPPDATALENGTH + sizeof(PacketHeader),
};

struct EncodeBuffer {
    enum { LENGTH = MAXAPPPACKETLENGTH };

    using Buffer_t = NS_SHARE::PieceBuffer<LENGTH>;
    
    EncodeBuffer() {}
    
    Buffer_t m_resultBuffer;
};

struct DecodeBuffer {
    enum { LENGTH = MAXAPPPACKETLENGTH };

    using Buffer_t = NS_SHARE::PieceBuffer<LENGTH>;

    DecodeBuffer() {}

    Buffer_t m_resultBuffer;
};

struct ThreadEncodeBuffer {
    enum { TAG = 0xFFEEDDCC };

    ThreadEncodeBuffer()
        : m_tag(TAG) {}

    uint32_t m_tag;
    EncodeBuffer m_buffer;
};

struct ThreadDecodeBuffer {
    enum { TAG = 0xFFEEDDCC };

    ThreadDecodeBuffer()
        : m_tag(TAG) {}

    uint32_t m_tag;
    DecodeBuffer m_buffer;
};

using PacketCache_t = NS_SHARE::StepBuffer<1024 * 4, 1024 * 256>;

using PacketArray_t = NS_SHARE::StaticArray<void*>;

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_PACKETDEFINE_HPP_
