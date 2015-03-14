#ifndef _MZ_SHARE_PIECEBUFFER_HPP_
#define _MZ_SHARE_PIECEBUFFER_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int MAXLENGTH>
class PieceBuffer {
    MZ_DISALLOW_COPY_AND_ASSIGN(PieceBuffer)

public:
    PieceBuffer()
        : m_pushIndex(0)
        , m_pullIndex(0) { memset(m_buffer, 0, sizeof(m_buffer)); }

    ~PieceBuffer() {}

public:
    void Reset() {
        memset(m_buffer, 0, sizeof(m_buffer));
        m_pushIndex = 0;
        m_pullIndex = 0;
    }

    int Push(const void* data, int length) {
        auto pushLength = this->GetPushLength();
        if (pushLength <= 0) {
            return 0;
        }

        if (pushLength < length) {
            length = pushLength;
        }
        memcpy(&m_buffer[m_pushIndex], data, length);
        m_pushIndex += length;
        return length;
    }

    int Pull(void* data, int length) {
        const auto pullLength = GetPullLength();
        if (pullLength <= 0) {
            return 0;
        }

        if (pullLength < length) {
            length = pullLength;
        }
        memcpy(data, &m_buffer[m_pullIndex], length);
        m_pullIndex += length;
        return length;
    }

    int Peek(void* data, int length) {
        const auto pullLength = GetPullLength();
        if (pullLength <= 0) {
            return 0;
        }

        if (pullLength < length) {
            length = pullLength;
        }
        memcpy(data, &m_buffer[m_pullIndex], length);
        return length;
    }

    int Skip(int length) {
        const auto pullLength = GetPullLength();
        if (pullLength <= 0) {
            return 0;
        }

        if (pullLength < length) {
            length = pullLength;
        }
        m_pullIndex += length;
        return length;
    }

    void Compact() {
        if (m_pullIndex == 0) {
            return;
        }

        if(m_pushIndex > m_pullIndex) {
            memmove(&m_buffer[0], &m_buffer[m_pullIndex], m_pushIndex-m_pullIndex);
            m_pushIndex = m_pushIndex - m_pullIndex;
            m_pullIndex = 0;
        } else  {
            m_pushIndex = 0;
            m_pullIndex = 0;
        }
    }

    int GetCapacity() const {
        return MAXLENGTH;
    }

    char* GetBuffer() {
        return m_buffer;
    }

    const char* GetBuffer() const {
        return m_buffer;
    }

    int GetPullLength() const {
        return m_pushIndex - m_pullIndex;
    }

    void SetPullIndex(int pullIndex) {
        m_pullIndex = pullIndex;
    }

    void JumpPullIndex(int pullOffset) {
        m_pullIndex += pullOffset;
    }

    char* GetPullBuffer() {
        return &m_buffer[m_pullIndex];
    }

    const char* GetPullBuffer() const {
        return &m_buffer[m_pullIndex];
    }

    int GetPushLength() const {
        return MAXLENGTH - m_pushIndex;
    }

    void SetPushIndex(int pushIndex) {
        m_pushIndex = pushIndex;
    }

    void JumpPushIndex(int pushOffset) {
        m_pushIndex += pushOffset;
    }

    char* GetPushBuffer() {
        return &m_buffer[m_pushIndex];
    }

    const char* GetPushBuffer() const {
        return &m_buffer[m_pushIndex];
    }

private:
    char m_buffer[MAXLENGTH];
    int m_pushIndex;
    int m_pullIndex;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_PIECEBUFFER_HPP_
