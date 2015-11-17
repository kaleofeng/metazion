#ifndef _MZ_SHARE_MEMORYINPUTSTREAM_HPP_
#define _MZ_SHARE_MEMORYINPUTSTREAM_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MemoryInputStream {
    MZ_DISALLOW_COPY_AND_ASSIGN(MemoryInputStream);

private:
    const char* m_buffer = nullptr;
    int m_length = 0;
    int m_position = 0;

public:
    MemoryInputStream() {}

    ~MemoryInputStream() {}

public:
    void Attach(const void* buffer, int length) {
        MZ_ASSERT_TRUE(!IsNull(buffer));
        MZ_ASSERT_TRUE(length > 0);

        m_buffer = static_cast<const char*>(buffer);
        m_length = length;
        m_position = 0;
    }

    void Detach() {
        m_buffer = nullptr;
        m_length = 0;
        m_position = 0;
    }

    void Seek(int position) {
        MZ_ASSERT_TRUE(position >= 0 && position < m_length);

        m_position = position;
    }

    bool ReadInt8(int8_t& arg) {
        return Read(&arg, sizeof(arg));
    }

    bool ReadUint8(uint8_t& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadInt16(int16_t& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadUint16(uint16_t& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadInt32(int32_t& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadUint32(uint32_t& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadInt64(int64_t& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadUint64(uint64_t& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadFloat(float& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadDouble(double& arg) {
        return Read(&arg, sizeof(arg));
    }
    
    bool ReadString(char* buffer, int length) {
        MZ_ASSERT_TRUE(!IsNull(buffer));
        MZ_ASSERT_TRUE(length > 0);

        int strLength = 0;
        if (!ReadInt32(strLength)) {
            return false;
        }

        if (length < strLength + 1) {
            return false;
        }

        if (strLength == 0) {
            buffer[0] = '\0';
            return true;
        }

        if (!Read(buffer, strLength)) {
            return false;
        }

        buffer[strLength] = '\0';
        return true;
    }
    
    bool Read(void* buffer, int length) {
        if (!Check(length)) {
            return false;
        }

        memcpy(buffer, m_buffer + m_position, length);
        m_position += length;
        return true;
    }


private:
    bool Check(int length) const {
        return m_position + length <= m_length;
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_MEMORYINPUTSTREAM_HPP_
