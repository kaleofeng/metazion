#ifndef _MZ_SHARE_MEMORYOUTPUTSTREAM_HPP_
#define _MZ_SHARE_MEMORYOUTPUTSTREAM_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int MAXLENGTH = 1024>
class MemoryOutputStream {
    MZ_DISALLOW_COPY_AND_ASSIGN(MemoryOutputStream);

private:
    char m_buffer[MAXLENGTH]{ 0 };
    int m_position{ 0 };

public:
    MemoryOutputStream() {}

    ~MemoryOutputStream() {}

public:
    const char* GetBuffer() const {
        return m_buffer;
    }

    int GetLength() const {
        return m_position;
    }

    void Seek(int position) {
        MZ_ASSERT_TRUE(position >= 0 && position < MAXLENGTH);

        m_position = position;
    }

    bool WriteInt8(int8_t value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteUint8(uint8_t value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteInt16(int16_t value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteUint16(uint16_t value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteInt32(int32_t value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteUint32(uint32_t value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteInt64(int64_t value) {
        return Write(&value, sizeof(value));
    }

    bool WriteUint64(uint64_t value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteFloat(float value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteDouble(double value) {
        return Write(&value, sizeof(value));
    }
    
    bool WriteString(const char* buffer, int length) {
        MZ_ASSERT_TRUE(!IsNull(buffer));
        MZ_ASSERT_TRUE(length >= 0);

        if (!WriteInt16(length)) {
            return true;
        }

        if (length == 0) {
            return true;
        }

        if (!Write(buffer, length)) {
            return false;
        }

        return true;
    }

    bool WriteString32(const char* buffer, int length) {
        MZ_ASSERT_TRUE(!IsNull(buffer));
        MZ_ASSERT_TRUE(length >= 0);

        if (!WriteInt32(length)) {
            return true;
        }

        if (length == 0) {
            return true;
        }

        if (!Write(buffer, length)) {
            return false;
        }

        return true;
    }

    bool Write(const void* buffer, int length) {
        if (!Check(length)) {
            return false;
        }

        memcpy(m_buffer + m_position, buffer, length);
        m_position += length;
        return true;
    }

private:
    bool Check(int length) const {
        return m_position + length <= MAXLENGTH;
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_MEMORYOUTPUTSTREAM_HPP_
