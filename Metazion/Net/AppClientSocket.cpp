#include "Metazion/Net/AppClientSocket.hpp"

#include "Metazion/Net/AppSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

AppClientSocket::AppClientSocket() {}

AppClientSocket::~AppClientSocket() {}

void AppClientSocket::Reset() {
    NormalClientSocket::Reset();
    m_packetSpecific.Reset();
}

void AppClientSocket::OnAttached() {
    NormalClientSocket::OnAttached();

    SocketServer* server = GetSocketServer();
    AppSocketServer* appServer = static_cast<AppSocketServer*>(server);
    ASSERT_TRUE(!IsNull(appServer));

    UnpackBuffer::BufferPool_t& unpackBufferPool = appServer->GetUnpackBufferPool();
    m_packetSpecific.SetUnpackBufferPool(unpackBufferPool);
    PacketCache_t::BufferPool_t& packetCachePool = appServer->GetPacketCachePool();
    m_packetSpecific.SetPacketCachePool(packetCachePool);
}

void AppClientSocket::OnStarted() {
    NormalClientSocket::OnStarted();
    m_packetSpecific.Rework();
}

int AppClientSocket::OnRecvData(const void* data, int length) {
    NormalClientSocket::OnRecvData(data, length);

    int processLength = 0;
    const char* curBuffer = static_cast<const char*>(data);
    int curLength = length;
    while (curLength > sizeof(PacketHeader)) {
        const int packetLength = m_packetSpecific.CheckPacketLength(curBuffer, curLength);
        if (packetLength < 0) {
            return -1;
        }
        else if (packetLength == 0) {
            break;
        }

        if (!m_packetSpecific.Unpack(curBuffer, packetLength)) {
            return -2;
        }

        curBuffer += packetLength;
        curLength -= packetLength;
        processLength += packetLength;
    }

    return processLength;
}

bool AppClientSocket::SendData(int command, const void* data, int length) {
    PackBuffer& packBuffer = GetPackBuffer();
    if (!m_packetSpecific.Pack(command, data, length, packBuffer)) {
        return false;
    }

    const void* pullBuffer = packBuffer.m_resultBuffer.GetPullBuffer();
    const int pullLength = packBuffer.m_resultBuffer.GetPullLength();
    return Send(pullBuffer, pullLength) == pullLength;
}

bool AppClientSocket::SendDataNow(int command, const void* data, int length) {
    PackBuffer& packBuffer = GetPackBuffer();
    if (!m_packetSpecific.Pack(command, data, length, packBuffer)) {
        return false;
    }

    const void* pullBuffer = packBuffer.m_resultBuffer.GetPullBuffer();
    const int pullLength = packBuffer.m_resultBuffer.GetPullLength();
    return SendNow(pullBuffer, pullLength) == pullLength;
}

int AppClientSocket::PullPackets(void* buffer, int length, PacketArray_t& packetArray) {
    return m_packetSpecific.PullPackets(buffer, length, packetArray);
}

PackBuffer& AppClientSocket::GetPackBuffer() {
    SocketServer* server = GetSocketServer();
    AppSocketServer* appServer = static_cast<AppSocketServer*>(server);
    ASSERT_TRUE(!IsNull(appServer));
    return appServer->GetPackBuffer();
}

DECL_NAMESPACE_MZ_NET_END
