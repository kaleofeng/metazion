#ifndef _MZ_SHARE_MSGINPUTSTREAM_HPP_
#define _MZ_SHARE_MSGINPUTSTREAM_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MemoryInputStream {
    DISALLOW_COPY_AND_ASSIGN(MemoryInputStream);

public:
    MemoryInputStream();

    ~MemoryInputStream();

public:
    void Attach(const void* buffer, int length);

    void Detach();

    void Seek(int position);

    bool ReadInt8(int8_t& arg);

    bool ReadUint8(uint8_t& arg);
    
    bool ReadInt16(int16_t& arg);
    
    bool ReadUint16(uint16_t& arg);
    
    bool ReadInt32(int32_t& arg);
    
    bool ReadUint32(uint32_t& arg);
    
    bool ReadInt64(int64_t& arg);
    
    bool ReadUint64(uint64_t& arg);
    
    bool ReadFloat(float& arg);
    
    bool ReadDouble(double& arg);
    
    bool ReadString(char* buffer, int32_t length);
    
    bool Read(void* buffer, int32_t length);

private:
    bool Check(int length);

private:
    const char* m_buffer;
    int m_length;
    int m_position;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_MSGINPUTSTREAM_HPP_
