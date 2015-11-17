#ifndef _MZ_SHARE_MEMORYRECORDSET_HPP_
#define _MZ_SHARE_MEMORYRECORDSET_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MemoryRecordset {
    MZ_DISALLOW_COPY_AND_ASSIGN(MemoryRecordset)

public:
    using Handle = void*;

    struct Header {
        volatile int m_usedCount = 0;
        int m_firstFree = 0;
        int m_recordSize = 0;
        int m_capacity = 0;
    };

private:
    enum {
        INVALIDRECORDINDEX = 0x7FFFFFFF,
        RECORDALIGNLENGTH = MZ_DEFAULT_ALIGNMENT,
    };

    struct Record {
        Record() { memset(this, 0, sizeof(*this)); }

        union {
            int m_flag;
            char m_record[1];
        };
    };

private:
    Header m_personalHeader;
    Header* m_activeHeader = nullptr;
    char* m_buffer = nullptr;
    int m_length = 0;
    bool m_ownMemory = false;

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
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_MEMORYRECORDSET_HPP_
