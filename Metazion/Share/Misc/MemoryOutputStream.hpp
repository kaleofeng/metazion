#ifndef MZ_SHARE_MSGOUTPUTSTREAM_HPP
#define MZ_SHARE_MSGOUTPUTSTREAM_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MemoryOutputStream {
    DISALLOW_COPY_AND_ASSIGN(MemoryOutputStream);

public:
    MemoryOutputStream();

    ~MemoryOutputStream();

public:
    const char* GetBuffer() const;

    int GetLength() const;

    bool WriteInt8(int8_t value);
    bool WriteUint8(uint8_t value);
    bool WriteInt16(int16_t value);
    bool WriteUint16(uint16_t value);
    bool WriteInt32(int32_t value);
    bool WriteUint32(uint32_t value);
    bool WriteInt64(int64_t value);
    bool WriteUint64(uint64_t value);
    bool WriteFloat(float value);
    bool WriteDouble(double value);
    bool WriteString(const char* buffer, int32_t length);
    bool Write(const void* buffer, int32_t length);

private:
    void Close();

    bool Check(int length);

private:
    char* m_buffer;
    int m_length;
    int m_position;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_MSGOUTPUTSTREAM_HPP
