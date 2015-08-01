#ifndef _MZ_SHARE_STEPBUFFER_HPP_
#define _MZ_SHARE_STEPBUFFER_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <mutex>

#include "Metazion/Share/Collection/UDSelfList.hpp"
#include "Metazion/Share/Memory/PieceBuffer.hpp"
#include "Metazion/Share/Memory/ObjectPool.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int STEPLENGTH
, int MAXLENGTH
>
class StepBuffer {
    MZ_DISALLOW_COPY_AND_ASSIGN(StepBuffer)

    enum { MAXSTEPNUMBER = MAXLENGTH + (STEPLENGTH - 1) / STEPLENGTH };

public:
    using Buffer_t = PieceBuffer<STEPLENGTH>;
    using BufferNode_t = UDSelfListNode<Buffer_t>;
    using BufferList_t = UDSelfList<BufferNode_t>;
    using BufferPool_t = ObjectPool<BufferNode_t, StepAllocator<256>, std::mutex>;

public:
    StepBuffer() {}

    ~StepBuffer() { Reset(); }

public:
    void SetBufferPool(BufferPool_t* bufferPool) {
        m_bufferPool = bufferPool;
    }

    void Reset() {
        auto buffer = m_bufferList.Front();
        while (!IsNull(buffer)) {
            m_bufferList.PopFront();
            m_bufferPool->Return(buffer);
            buffer = m_bufferList.Front();
        }
        m_bufferPool = nullptr;
        m_stepNumber = 0;
        m_length = 0;
    }

    int Push(const void* data, int length) {
        auto tData = static_cast<const char*>(data);;
        auto tLength = length;
        while (tLength > 0) {
            auto buffer = m_bufferList.Back();
            const int pushLength = _Push(buffer, tData, tLength);
            if (pushLength <= 0) {
                if (!Inflate()) {
                    break;
                }
            }
            tData += pushLength;
            tLength -= pushLength;
        }
        return length - tLength;
    }

    int Pull(void* data, int length) {
        auto tData = static_cast<char*>(data);;
        auto tLength = length;
        while (tLength > 0) {
            auto buffer = m_bufferList.Front();
            const int pullLength = _Pull(buffer, tData, tLength);
            if (pullLength <= 0) {
                break;
            }
            tData += pullLength;
            tLength -= pullLength;
            Compact();
        }
        return length - tLength;
    }

    int Peek(void* data, int length) {
        auto tData = static_cast<char*>(data);;
        auto tLength = length;
        auto buffer = m_bufferList.Front();
        while (tLength > 0) {
            const int peekLength = _Peek(buffer, tData, tLength);
            if (peekLength <= 0) {
                break;
            }
            tData += peekLength;
            tLength -= peekLength;
            buffer = m_bufferList.Forward(buffer);
        }
        return length - tLength;
    }

    int Skip(int length) {
        auto tLength = length;
        while (tLength > 0) {
            auto buffer = m_bufferList.Front();
            const auto skipLength = _Skip(buffer, tLength);
            if (skipLength <= 0) {
                break;
            }
            tLength -= skipLength;
            Compact();
        }
        return length - tLength;
    }

    bool IsEmpty() const {
        return m_length == 0;
    }

    bool IsFull() const {
        return MAXLENGTH == m_length;
    }

    int GetCapacity() const {
        return MAXLENGTH;
    }

    int GetPushLength() const {
        return MAXLENGTH - m_length;
    }

    int GetPullLength() const {
        return m_length;
    }

private:
    int _Push(BufferNode_t* buffer, const void* data, int length) {
        if (IsNull(buffer)) {
            return 0;
        }

        const auto pushLength = buffer->m_value.Push(data, length);
        m_length += pushLength;
        return pushLength;
    }

    int _Pull(BufferNode_t* buffer, void* data, int length) {
        if (IsNull(buffer)) {
            return 0;
        }

        const auto pullLength = buffer->m_value.Pull(data, length);
        m_length -= pullLength;
        return pullLength;
    }

    int _Peek(BufferNode_t* buffer, void* data, int length) {
        if (IsNull(buffer)) {
            return 0;
        }

        return buffer->m_value.Peek(data, length);
    }

    int _Skip(BufferNode_t* buffer, int length) {
        if (IsNull(buffer)) {
            return 0;
        }

        const auto skipLength = buffer->m_value.Skip(length);
        m_length -= skipLength;
        return skipLength;
    }

    bool Inflate() {
        if (m_stepNumber >= MAXSTEPNUMBER) {
            return false;
        }

        auto buffer = m_bufferPool->Obtain();
        if (IsNull(buffer)) {
            return false;
        }

        buffer->m_value.Reset();
        m_bufferList.PushBack(buffer);
        ++m_stepNumber;
        return true;
    }

    void Compact() {
        auto buffer = m_bufferList.Front();
        if (IsNull(buffer)) {
            return;
        }

        if (buffer->m_value.GetPullLength() > 0) {
            return;
        }

        if (m_bufferList.GetSize() == 1) {
            buffer->m_value.Reset();
            return;
        }

        m_bufferList.PopFront();
        m_bufferPool->Return(buffer);
        --m_stepNumber;
    }

private:
    BufferList_t m_bufferList;
    BufferPool_t* m_bufferPool = nullptr;
    int m_stepNumber = 0;
    int m_length = 0;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_STEPBUFFER_HPP_
