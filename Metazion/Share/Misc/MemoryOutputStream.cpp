#include "Metazion/Share/Misc/MemoryOutputStream.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

MemoryOutputStream::MemoryOutputStream()
    : m_buffer(nullptr)
    , m_length(0)
    , m_position(0) {}

MemoryOutputStream::~MemoryOutputStream() { Close(); }

const char* MemoryOutputStream::GetBuffer() const {
    return m_buffer;
}

int MemoryOutputStream::GetLength() const {
    return m_position;
}

void MemoryOutputStream::Seek(int position) {
    m_position = 0;
}

bool MemoryOutputStream::WriteInt8(int8_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteUint8(uint8_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteInt16(int16_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteUint16(uint16_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteInt32(int32_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteUint32(uint32_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteInt64(int64_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteUint64(uint64_t value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteFloat(float value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteDouble(double value) {
    return Write(&value, sizeof(value));
}

bool MemoryOutputStream::WriteString(const char* buffer, int32_t length) {
    if (0 == length) {
        length = static_cast<int32_t>(strlen(buffer));
    }

    if (!WriteInt32(length)) {
        return true;
    }

    if (0 == length) {
        return true;
    }

    if (!Write(buffer, length)) {
        return false;
    }

    return true;
}

bool MemoryOutputStream::Write(const void* buffer, int32_t length) {
    if (!Check(length)) {
        return false;
    }

    ::memcpy(m_buffer + m_position, buffer, length);
    m_position += length;
    return true;
}

bool MemoryOutputStream::Check(int length) {
    const auto totalLength = m_position + length;
    if (totalLength <= m_length) {
        return true;
    }
    
    const auto alignLength = AlignLength(totalLength, 1024);
    if (IsNull(m_buffer)) {
        m_buffer = new char[alignLength];
        m_length = alignLength;
        return true;
    }

    auto buffer = new char[alignLength];
    ::memcpy(m_buffer, m_buffer, m_position);
    m_buffer = buffer;
    m_length = alignLength;
    return true;
}

void MemoryOutputStream::Close() {
    SafeDeleteArray(m_buffer);
    m_length = 0;
    m_position = 0;
}

DECL_NAMESPACE_MZ_SHARE_END
