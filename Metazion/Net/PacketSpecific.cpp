#include "Metazion/Net/PacketSpecific.hpp"

#include <Metazion/Share/Sync/AutoGuard.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

PacketSpecific::PacketSpecific()
    : m_unpackBufferPool(nullptr)
    , m_packetCachePool(nullptr) {}

PacketSpecific::~PacketSpecific() {}

void PacketSpecific::SetUnpackBufferPool(UnpackBuffer::BufferPool_t& bufferPool) {
    m_unpackBufferPool = &bufferPool;
    m_unpackBuffer.m_bufferPool = m_unpackBufferPool;
}

void PacketSpecific::SetPacketCachePool(PacketCache_t::BufferPool_t& bufferPool) {
    m_packetCachePool = &bufferPool;
    m_packetCache.SetBufferPool(m_packetCachePool);
}
void PacketSpecific::Reset() {
    m_unpackBuffer.m_buffer = nullptr;
    m_unpackBuffer.m_bufferPool = nullptr;
    m_unpackBufferPool = nullptr;
    m_packetCache.Reset();
    m_packetCachePool = nullptr;
}

void PacketSpecific::Rework() {
    m_unpackBuffer.m_buffer = nullptr;
    m_unpackBuffer.m_bufferPool = m_unpackBufferPool;
    m_packetCache.Reset();
    m_packetCache.SetBufferPool(m_packetCachePool);
}

bool PacketSpecific::Pack(int command, const void* data, int length, PackBuffer& packBuffer) {
    packBuffer.m_tempBuffer.Reset();
    packBuffer.m_resultBuffer.Reset();

    if (length < 0) {
        ASSERT_TRUE(false);
        return false;
    }

    if (length > MAXAPPDATALENGTH) {
        ASSERT_TRUE(false);
        return false;
    }

    PacketHeader header;
    header.m_command = command;
    header.m_length = length;
    
    if (length <= MAXNETDATALENGTH) {
        packBuffer.m_resultBuffer.Push(&header, sizeof(header));
        packBuffer.m_resultBuffer.Push(data, header.m_length);
        return true;
    }
    
    packBuffer.m_tempBuffer.Push(&header, sizeof(header));
    packBuffer.m_tempBuffer.Push(data, header.m_length);

    const char* restData = packBuffer.m_tempBuffer.GetPullBuffer();
    int restLength = packBuffer.m_tempBuffer.GetPullLength();
    bool first = true;
    while (restLength > MAXNETDATALENGTH) {
        PacketHeader header;
        header.m_command = first ? PacketHeader::MULTIPLEBEGIN : PacketHeader::MULTIPLE;
        header.m_length = MAXNETDATALENGTH;
        packBuffer.m_resultBuffer.Push(&header, sizeof(header));
        packBuffer.m_resultBuffer.Push(restData, header.m_length);
        restData += header.m_length;
        restLength -= header.m_length;
        first = false;
    }
    
    ASSERT_TRUE(restLength > 0);
    header.m_command = PacketHeader::MULTIPLEEND;
    header.m_length = restLength;
    packBuffer.m_resultBuffer.Push(&header, sizeof(header));
    packBuffer.m_resultBuffer.Push(restData, header.m_length);
    return true;
}

bool PacketSpecific::Unpack(const void* data, int length) {
    const PacketHeader* header = static_cast<const PacketHeader*>(data);
    ASSERT_TRUE(length == sizeof(PacketHeader) + header->m_length);

    if (header->m_command == PacketHeader::MULTIPLEBEGIN) {
        ASSERT_TRUE(IsNull(m_unpackBuffer.m_buffer));
        if (IsNull(m_unpackBuffer.m_buffer)) {
            m_unpackBuffer.m_buffer = m_unpackBuffer.m_bufferPool->Obtain();
        }
        
        ASSERT_TRUE(!IsNull(m_unpackBuffer.m_buffer));
        m_unpackBuffer.m_buffer->Reset();
        int pushLength = m_unpackBuffer.m_buffer->Push(header + 1, header->m_length);
        if (pushLength < header->m_length) {
            m_unpackBuffer.m_bufferPool->Return(m_unpackBuffer.m_buffer);
            m_unpackBuffer.m_buffer = nullptr;
            return false;
        }
    }
    else if (header->m_command == PacketHeader::MULTIPLE) {
        ASSERT_TRUE(!IsNull(m_unpackBuffer.m_buffer));
        int pushLength = m_unpackBuffer.m_buffer->Push(header + 1, header->m_length);
        if (pushLength < header->m_length) {
            m_unpackBuffer.m_bufferPool->Return(m_unpackBuffer.m_buffer);
            m_unpackBuffer.m_buffer = nullptr;
            return false;
        }
    }
    else if (header->m_command == PacketHeader::MULTIPLEEND) {
        ASSERT_TRUE(!IsNull(m_unpackBuffer.m_buffer));
        int pushLength = m_unpackBuffer.m_buffer->Push(header + 1, header->m_length);
        if (pushLength < header->m_length) {
            m_unpackBuffer.m_bufferPool->Return(m_unpackBuffer.m_buffer);
            m_unpackBuffer.m_buffer = nullptr;
            return false;
        }
        
        const void* appData = m_unpackBuffer.m_buffer->GetPullBuffer();
        const int appLength = m_unpackBuffer.m_buffer->GetPullLength();
        PushPacket(appData, appLength);
        
        m_unpackBuffer.m_bufferPool->Return(m_unpackBuffer.m_buffer);
        m_unpackBuffer.m_buffer = nullptr;
    }
    else {
        ASSERT_TRUE(IsNull(m_unpackBuffer.m_buffer));
        PushPacket(data, length);
    }

    return true;
}

int PacketSpecific::PullPackets(void* buffer, int length, PacketArray_t& packetArray) {
    packetArray.Clear();
    
    m_lock.Lock();
    char* curBuffer = static_cast<char*>(buffer);
    int curLength = length;
    while (true) {
        const int packetLength = _PullPacket(curBuffer, length);
        if (packetLength <= 0) {
            break;
        }

        packetArray.Add(curBuffer);
        curBuffer += packetLength;
        curLength -= packetLength;
    }
    m_lock.Unlock();
    
    return packetArray.GetSize();
}

int PacketSpecific::CheckPacketLength(const void* data, int length) {
    const PacketHeader* packetHeader = static_cast<const PacketHeader*>(data);
    ASSERT_TRUE(!IsNull(packetHeader));

    const int packetLength = sizeof(PacketHeader) + packetHeader->m_length;
    if (packetLength > MAXNETPACKETLENGTH) {
        return -1;
    }

    if (length < packetLength) {
        return 0;
    }

    return packetLength;
}

int PacketSpecific::PushPacket(const void* data, int length) {
    NS_SHARE::AutoGuard<NS_SHARE::MutexLock> autoGuard(m_lock);
    return _PushPacket(data, length);
}

int PacketSpecific::PullPacket(void* data, int length) {
    NS_SHARE::AutoGuard<NS_SHARE::MutexLock> autoGuard(m_lock);
    return _PullPacket(data, length);
}

int PacketSpecific::_PushPacket(const void* data, int length) {
    const int pushLength = m_packetCache.GetPushLength();
    if (pushLength < length) {
        return 0;
    }
    
    const int realLength = m_packetCache.Push(data, length);
    ASSERT_TRUE(realLength == length);
    return realLength;
}

int PacketSpecific::_PullPacket(void* data, int length) {
    PacketHeader packetHeader;
    const int peekLength = m_packetCache.Peek(&packetHeader, sizeof(packetHeader));
    if (peekLength < sizeof(packetHeader)) {
        return 0;
    }

    const int packetLength = sizeof(packetHeader) + packetHeader.m_length;
    if (length < packetLength) {
        return 0;
    }

    const int realLength = m_packetCache.Pull(data, packetLength);
    ASSERT_TRUE(realLength == packetLength);
    return realLength;
}

DECL_NAMESPACE_MZ_NET_END
