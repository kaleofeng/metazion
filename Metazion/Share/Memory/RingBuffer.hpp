#ifndef _MZ_SHARE_RINGBUFFER_HPP_
#define _MZ_SHARE_RINGBUFFER_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int MAXLENGTH>
class RingBuffer {
    MZ_DISALLOW_COPY_AND_ASSIGN(RingBuffer)

public:
    RingBuffer() {}

    ~RingBuffer() {}

public:
    void Reset() {
        memset(m_buffer, 0, sizeof(m_buffer));
        m_length = 0;
        m_pushIndex = 0;
        m_pullIndex = 0;
    }

    int Push(const void* data, int length) {
        const auto buffer = static_cast<const char*>(data);
        const auto vacant = GetPushLength();
        const auto properLength = vacant < length ? vacant : length;
        if (properLength <= 0) {
            return 0;
        }

        const auto rightLength = MAXLENGTH - m_pushIndex;
        if (properLength <= rightLength) {
            memcpy(&m_buffer[m_pushIndex], buffer, properLength);
            m_pushIndex += properLength;
        }
        else {
            memcpy(&m_buffer[m_pushIndex], buffer, rightLength);
            memcpy(m_buffer, &buffer[rightLength], properLength - rightLength);
            m_pushIndex = properLength - rightLength;
        }

        m_pushIndex %= MAXLENGTH;
        m_length += properLength;
        return properLength;
    }

    int Pull(void* data, int length) {
        auto buffer = static_cast<char*>(data);

        const auto engaged = GetPullLength();
        const auto properLength = engaged < length ? engaged : length;
        if (properLength <= 0) {
            return 0;
        }

        const auto rightLength = MAXLENGTH - m_pullIndex;
        if (properLength <= rightLength) {
            memcpy(buffer, &m_buffer[m_pullIndex], properLength);
            m_pullIndex += properLength;
        }
        else {
            memcpy(buffer, &m_buffer[m_pullIndex], rightLength);
            memcpy(&buffer[rightLength], m_buffer, properLength - rightLength);
            m_pullIndex = properLength - rightLength;
        }

        m_pullIndex %= MAXLENGTH;
        m_length -= properLength;
        return properLength;
    }
    
    int Peek(void* data, int length) {
        auto buffer = static_cast<char*>(data);

        const auto engaged = GetPullLength();
        const auto properLength = engaged < length ? engaged : length;
        if (properLength <= 0) {
            return 0;
        }

        const auto rightLength = MAXLENGTH - m_pullIndex;
        if (properLength <= rightLength) {
            memcpy(buffer, &m_buffer[m_pullIndex], properLength);
        }
        else {
            memcpy(buffer, &m_buffer[m_pullIndex], rightLength);
            memcpy(&buffer[rightLength], m_buffer, properLength - rightLength);
        }

        return properLength;
    }

    int Skip(int length) {
        const auto engaged = GetPullLength();
        const auto properLength = engaged < length ? engaged : length;
        if (properLength <= 0) {
            return 0;
        }

        m_pullIndex = (m_pullIndex + properLength) % MAXLENGTH;
        m_length -= properLength;
        return properLength;
    }

    bool IsEmpty() const { return m_length == 0; }

    bool IsFull() const { return m_length == MAXLENGTH; }

    int GetCapacity() const { return MAXLENGTH; }
    
    int GetPushLength() const { return MAXLENGTH - m_length; }

    int GetPullLength() const { return m_length; }

private:
    char m_buffer[MAXLENGTH] = { 0 };
    int m_length = 0;
    int m_pushIndex = 0;
    int m_pullIndex = 0;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_RINGBUFFER_HPP_
