#include "Metazion/Net/ComPacketer.hpp"

#include "Metazion/Net/TransmitSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

ComPacketer::ComPacketer(TransmitSocket& socket)
    : m_socket(socket)
    , m_packetCodec(socket) {}

ComPacketer::~ComPacketer() {}

void ComPacketer::Reset() {
    SetValidPacketCallback(nullptr);
    SetInvalidPacketCallback(nullptr);
}

void ComPacketer::Dispatch() {
    thread_local DecodeBuffer decodeBuffer;

    while (true) {
        int command = 0;
        decodeBuffer.m_resultBuffer.Reset();
        const auto ret = m_packetCodec.Decode(command, decodeBuffer);
        if (ret == 0) {
            break;
        }
        else if (ret < 0) {
            m_invalidPacketCallback();
            break;
        }

        const auto pullBuffer = decodeBuffer.m_resultBuffer.GetPullBuffer();
        const auto pullLength = decodeBuffer.m_resultBuffer.GetPullLength();
        m_validPacketCallback(command, pullBuffer, pullLength);
    }
}

bool ComPacketer::SendData(int command, const void* data, int length) {
    thread_local EncodeBuffer encodeBuffer;
    encodeBuffer.m_resultBuffer.Reset();

    const auto ret = m_packetCodec.Encode(command, data, length, encodeBuffer);
    if (ret <= 0) {
        return false;
    }
        
    const auto pullBuffer = encodeBuffer.m_resultBuffer.GetPullBuffer();
    const auto pullLength = encodeBuffer.m_resultBuffer.GetPullLength();
    return m_socket.Send(pullBuffer, pullLength) == pullLength;
}

void ComPacketer::PostData(int command, const void* data, int length) {
    m_validPacketCallback(command, data, length);
}

DECL_NAMESPACE_MZ_NET_END
