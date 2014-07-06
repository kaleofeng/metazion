#include "Metazion/Net/PacketSpecific.hpp"

#include "Metazion/Net/TransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

PacketSpecific::PacketSpecific(TransmitSocket& transmitSocket)
    : m_transmitSocket(transmitSocket) {}

PacketSpecific::~PacketSpecific() {}

int PacketSpecific::Encode(int command, const void* data, int length, EncodeBuffer& encodeBuffer) {
    if (length < 0) {
        ASSERT_TRUE(false);
        return -1;
    }

    if (length > MAXAPPDATALENGTH) {
        ASSERT_TRUE(false);
        return -2;
    }

    PacketHeader header;
    header.m_command = command;
    header.m_length = length;
    
    encodeBuffer.m_resultBuffer.Push(&header, sizeof(header));
    encodeBuffer.m_resultBuffer.Push(data, header.m_length);
    return sizeof(header) + header.m_length;
}

int PacketSpecific::Decode(int& command, DecodeBuffer& decodeBuffer) {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    PacketHeader header;
    const auto headerLength = static_cast<int>(sizeof(header));
    const auto peekLength = socketBuffer.m_recvCache.Peek(&header, headerLength);
    if (peekLength != headerLength) {
        return 0;
    }

    const auto packetLength = static_cast<int>(sizeof(header)+header.m_length);
    if (packetLength > MAXAPPPACKETLENGTH) {
        return -1;
    }

    const auto cacheLength = socketBuffer.m_recvCache.GetPullLength();
    if (cacheLength < packetLength) {
        return 0;
    }

    auto pushBuffer = decodeBuffer.m_resultBuffer.GetPushBuffer();
    const auto pushLength = decodeBuffer.m_resultBuffer.GetPushLength();
    ASSERT_TRUE(pushLength >= packetLength);

    socketBuffer.m_recvLock.lock();

    const auto pullHeaderLength = socketBuffer.m_recvCache.Pull(&header, headerLength);
    ASSERT_TRUE(pullHeaderLength == headerLength);

    const auto pullDataLength = socketBuffer.m_recvCache.Pull(pushBuffer, packetLength);
    ASSERT_TRUE(pullDataLength == header.m_length);

    socketBuffer.m_recvLock.unlock();

    command = header.m_command;
    decodeBuffer.m_resultBuffer.SetPushIndex(pullDataLength);
    return header.m_length;
}

DECL_NAMESPACE_MZ_NET_END
