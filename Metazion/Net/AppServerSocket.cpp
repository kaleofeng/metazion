#include "Metazion/Net/AppServerSocket.hpp"

#include "Metazion/Net/AppSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

AppServerSocket::AppServerSocket()
    : m_packetSpecific(*this) {}

AppServerSocket::~AppServerSocket() {}

void AppServerSocket::Dispatch() {
    DecodeBuffer& decodeBuffer = GetDecodeBuffer();

    while (true) {
        int command = 0;
        decodeBuffer.m_resultBuffer.Reset();
        const int ret = m_packetSpecific.Decode(command, decodeBuffer);
        if (0 == ret) {
            break;
        }
        else if (ret < 0) {
            OnInvalidPacket();
            break;
        }

        const char* pullBuffer = decodeBuffer.m_resultBuffer.GetPullBuffer();
        const int pullLength = decodeBuffer.m_resultBuffer.GetPullLength();
        OnValidPacket(command, pullBuffer, pullLength);
    }
}

bool AppServerSocket::SendData(int command, const void* data, int length) {
    EncodeBuffer& encodeBuffer = GetEncodeBuffer();
    encodeBuffer.m_resultBuffer.Reset();

    const int ret = m_packetSpecific.Encode(command, data, length, encodeBuffer);
    if (ret <= 0) {
        return false;
    }

    const void* pullBuffer = encodeBuffer.m_resultBuffer.GetPullBuffer();
    const int pullLength = encodeBuffer.m_resultBuffer.GetPullLength();
    return Send(pullBuffer, pullLength) == pullLength;
}

EncodeBuffer& AppServerSocket::GetEncodeBuffer() {
    SocketServer* server = GetSocketServer();
    AppSocketServer* appServer = static_cast<AppSocketServer*>(server);
    ASSERT_TRUE(!IsNull(appServer));
    return appServer->GetEncodeBuffer();
}

DecodeBuffer& AppServerSocket::GetDecodeBuffer() {
    SocketServer* server = GetSocketServer();
    AppSocketServer* appServer = static_cast<AppSocketServer*>(server);
    ASSERT_TRUE(!IsNull(appServer));
    return appServer->GetDecodeBuffer();
}

DECL_NAMESPACE_MZ_NET_END
