#include "Metazion/Net/PacketCodec.hpp"

#include "Metazion/Net/TransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

PacketCodec::PacketCodec(TransmitSocket& transmitSocket)
    : m_transmitSocket(transmitSocket) {}

PacketCodec::~PacketCodec() {}

int PacketCodec::Encode(int command, const void* data, int length, EncodeBuffer& encodeBuffer) {
    if (length < 0) {
        MZ_ASSERT_TRUE(false);
        return -1;
    }

    if (length > MAXAPPDATALENGTH) {
        MZ_ASSERT_TRUE(false);
        return -2;
    }

    PacketHeader header;
    header.m_command = command;
    header.m_length = length;
    
    encodeBuffer.m_resultBuffer.Push(&header, sizeof(header));
    encodeBuffer.m_resultBuffer.Push(data, header.m_length);
    return sizeof(header) + header.m_length;
}

int PacketCodec::Decode(int& command, DecodeBuffer& decodeBuffer) {
    auto& socketBuffer = m_transmitSocket.GetSocketBuffer();

    PacketHeader header;
    const auto headerLength = static_cast<int>(sizeof(header));
    const auto peekLength = socketBuffer.m_recvCache.Peek(&header, headerLength);
    if (peekLength != headerLength) {
        return 0;
    }

    const auto packetLength = static_cast<int>(header.m_length);
    if (packetLength > MAXAPPPACKETLENGTH) {
        return -1;
    }

    const auto cacheLength = socketBuffer.m_recvCache.GetPullLength();
    if (cacheLength < headerLength + packetLength) {
        return 0;
    }

    auto pushBuffer = decodeBuffer.m_resultBuffer.GetPushBuffer();
    const auto pushLength = decodeBuffer.m_resultBuffer.GetPushLength();
    MZ_ASSERT_TRUE(pushLength >= packetLength);

    socketBuffer.m_recvLock.lock();

    const auto pullHeaderLength = socketBuffer.m_recvCache.Pull(&header, headerLength);
    MZ_ASSERT_TRUE(pullHeaderLength == headerLength);

    const auto pullDataLength = socketBuffer.m_recvCache.Pull(pushBuffer, packetLength);
    MZ_ASSERT_TRUE(pullDataLength == packetLength);

    socketBuffer.m_recvLock.unlock();

    command = header.m_command;
    decodeBuffer.m_resultBuffer.SetPushIndex(pullDataLength);
    return header.m_length;
}

DECL_NAMESPACE_MZ_NET_END
