#ifndef MZ_SHARE_MEMORYRECORDSET_HPP
#define MZ_SHARE_MEMORYRECORDSET_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class MemoryRecordset {
    DISALLOW_COPY_AND_ASSIGN(MemoryRecordset)

public:
    using Handle = void*;

    struct Header {
        Header();

        volatile int m_usedCount;
        int m_firstFree;
        int m_recordSize;
        int m_capacity;
    };

private:
    enum { INVALIDRECORDINDEX = 0x7fffffff };
    enum { RECORDALIGNLENGTH = DEFAULT_ALIGNMENT };

    struct Record {
        Record();

        union {
            int m_flag;
            char m_record[1];
        } m_data;
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

#endif // MZ_SHARE_MEMORYRECORDSET_HPP
