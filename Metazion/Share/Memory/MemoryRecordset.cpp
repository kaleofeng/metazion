#include "Metazion/Share/Memory/MemoryRecordset.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

MemoryRecordset::MemoryRecordset()
    : m_activeHeader(nullptr)
    , m_buffer(nullptr)
    , m_length(0)
    , m_ownMemory(false) {}

MemoryRecordset::~MemoryRecordset() {}

void MemoryRecordset::Initialize(int recordSize, int capacity) {
    MZ_ASSERT_TRUE(recordSize > 0);
    MZ_ASSERT_TRUE(capacity > 0);

    m_personalHeader.m_recordSize = AlignLength(recordSize, RECORDALIGNLENGTH);
    m_personalHeader.m_capacity = capacity;
    m_personalHeader.m_firstFree = INVALIDRECORDINDEX;
    m_personalHeader.m_usedCount = 0;

    m_length = m_personalHeader.m_recordSize * m_personalHeader.m_capacity;
    m_buffer = new char[m_length];

    m_activeHeader = &m_personalHeader;
    m_ownMemory = true;
}

void MemoryRecordset::Finalize() {
    MZ_ASSERT_TRUE(m_ownMemory);

    SafeDeleteArray(m_buffer);
    m_length = 0;
    m_activeHeader = nullptr;
    m_ownMemory = false;
}

void MemoryRecordset::Attach(Header& header, void* buffer) {
    MZ_ASSERT_TRUE(IsLengthAlign(header.m_recordSize, RECORDALIGNLENGTH));

    m_length = header.m_recordSize * header.m_capacity;
    m_buffer = reinterpret_cast<char*>(buffer);

    m_activeHeader = &header;
    m_ownMemory = false;
}

void MemoryRecordset::Detach() {
    MZ_ASSERT_TRUE(!m_ownMemory);

    m_length = 0;
    m_buffer = nullptr;
    m_activeHeader = nullptr;
    m_ownMemory = true;
}

void MemoryRecordset::Reset() {
    m_activeHeader->m_firstFree = INVALIDRECORDINDEX;
    m_activeHeader->m_usedCount = 0;
}

MemoryRecordset::Handle MemoryRecordset::ObtainRecord() {
    if (m_activeHeader->m_firstFree == INVALIDRECORDINDEX) {
        auto recordIndex = m_activeHeader->m_usedCount;
        if (!IsValidRecordIndex(recordIndex)) {
            return Handle(0);
        }

        Handle handle = RecordIndexToHandle(recordIndex);
        ++m_activeHeader->m_usedCount;
        return handle;
    }

    const auto recordIndex = m_activeHeader->m_firstFree;
    Handle handle = RecordIndexToHandle(recordIndex);
    auto memory = GetMemory(handle);
    auto record = static_cast<Record*>(memory);
    m_activeHeader->m_firstFree = record->m_flag;
    return handle;
}

bool MemoryRecordset::ReturnRecord(MemoryRecordset::Handle handle) {
    if (!IsValidHandle(handle)) {
        return false;
    }

    auto memory = GetMemory(handle);
    auto record = static_cast<Record*>(memory);
    record->m_flag = m_activeHeader->m_firstFree;
    m_activeHeader->m_firstFree = HandleToRecordIndex(handle);
    return true;
}

void* MemoryRecordset::GetMemory(MemoryRecordset::Handle handle) {
    if (!IsValidHandle(handle)) {
        return nullptr;
    }

    auto memory = reinterpret_cast<char*>(handle);
    return memory;
}

MemoryRecordset::Handle MemoryRecordset::GetHandle(void* memory) {
    return Handle(memory);
}

bool MemoryRecordset::IsValidHandle(MemoryRecordset::Handle handle) {
    auto memory = reinterpret_cast<char*>(handle);
    return memory >= m_buffer && memory < m_buffer + m_length;
}

bool MemoryRecordset::IsValidMomory(void* memory) {
    return memory >= m_buffer && memory < m_buffer + m_length;
}

int MemoryRecordset::HandleToRecordIndex(MemoryRecordset::Handle handle) {
    const auto memory = reinterpret_cast<const char*>(handle);
    const auto memoryLength = static_cast<intptr_t>(memory - m_buffer);
    return static_cast<int>(memoryLength / m_activeHeader->m_recordSize);
}

MemoryRecordset::Handle MemoryRecordset::RecordIndexToHandle(int index) {
    const auto memoryLength = m_activeHeader->m_recordSize * index;
    return Handle(m_buffer + memoryLength);
}

bool MemoryRecordset::IsValidRecordIndex(int index) {
    return index >= 0 && index < m_activeHeader->m_capacity;
}

DECL_NAMESPACE_MZ_SHARE_END
