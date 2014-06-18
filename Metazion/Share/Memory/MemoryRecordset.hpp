#ifndef _MZ_SHARE_MEMORYRECORDSET_HPP_
#define _MZ_SHARE_MEMORYRECORDSET_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MemoryRecordset {
    DISALLOW_COPY_AND_ASSIGN(MemoryRecordset)

public:
    using Handle = void*;

    struct Header {
        Header()
            : m_usedCount(0)
            , m_firstFree(0)
            , m_recordSize(0)
            , m_capacity(0) {}

        volatile int m_usedCount;
        int m_firstFree;
        int m_recordSize;
        int m_capacity;
    };

private:
    enum {
        INVALIDRECORDINDEX = 0x7FFFFFFF,
        RECORDALIGNLENGTH = DEFAULT_ALIGNMENT,
    };

    struct Record {
        Record() {
            ::memset(this, 0, sizeof(*this));
        }

        union {
            int m_flag;
            char m_record[1];
        };
    };

public:
    MemoryRecordset();

    ~MemoryRecordset();

public:
    void Initialize(int recordSize, int capacity);

    void Finalize();

    void Attach(Header& header, void* buffer);

    void Detach();

    void Reset();

    Handle ObtainRecord();

    bool ReturnRecord(Handle handle);

    void* GetMemory(Handle handle);

    Handle GetHandle(void* memory);

    bool IsValidHandle(Handle handle);

    bool IsValidMomory(void* memory);

private:
    int HandleToRecordIndex(Handle handle);

    Handle RecordIndexToHandle(int index);

    bool IsValidRecordIndex(int index);

private:
    Header m_personalHeader;
    Header* m_activeHeader;
    char* m_buffer;
    int m_length;
    bool m_ownMemory;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_MEMORYRECORDSET_HPP_
