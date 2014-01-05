#include "Metazion/Share/Misc/MemoryInputStream.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

MemoryInputStream::MemoryInputStream()
    : m_buffer(nullptr)
    , m_length(0)
    , m_position(0) {}

MemoryInputStream::MemoryInputStream(const void* buffer, int length)
    : m_buffer(nullptr)
    , m_length(0)
    , m_position(0) {
    Attach(buffer, length);
}

MemoryInputStream::~MemoryInputStream() {
    Detach();
}

void MemoryInputStream::Seek(int position) {
    m_position = position;
}

void MemoryInputStream::Attach(const void* buffer, int length) {
    m_buffer = static_cast<const char*>(buffer);
    m_length = length;
    m_position = 0;
}

void MemoryInputStream::Detach() {
    m_buffer = nullptr;
    m_length = 0;
    m_position = 0;
}

bool MemoryInputStream::ReadInt8(int8_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadUint8(uint8_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadInt16(int16_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadUint16(uint16_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadInt32(int32_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadUint32(uint32_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadInt64(int64_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadUint64(uint64_t& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadFloat(float& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadDouble(double& arg) {
    return Read(&arg, sizeof(arg));
}

bool MemoryInputStream::ReadString(char* buffer, int32_t length) {
    int32_t strLength = 0;
    if (!ReadInt32(strLength)) {
        return false;
    }

    if (length < strLength + 1) {
        return false;
    }

    if (0 == strLength) {
        buffer[0] = '\0';
        return true;
    }

    if (!Read(buffer, strLength)) {
         return false;
    }

    buffer[strLength] = '\0';
    return true;
}

bool MemoryInputStream::Read(void* buffer, int32_t length) {
    if (!Check(length)) {
        return false;
    }

    ::memcpy(buffer, m_buffer + m_position, length);
    m_position += length;
    return true;
}

bool MemoryInputStream::Check(int length) {
    return m_position + length <= m_length;
}

DECL_NAMESPACE_MZ_SHARE_END
