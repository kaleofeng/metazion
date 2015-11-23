#ifndef _MZ_NET_OUTPUTBUFFER_HPP_
#define _MZ_NET_OUTPUTBUFFER_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <mutex>

#include <Metazion/Share/Collection/UDSelfList.hpp>
#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/ObjectPool.hpp>

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int STEPLENGTH
, int MAXSIZE
>
class OutputBuffer {
    MZ_DISALLOW_COPY_AND_ASSIGN(OutputBuffer)

    enum { MAXLENGTH = STEPLENGTH * MAXSIZE };

public:
    using Buffer_t = PieceBuffer<STEPLENGTH>;
    using BufferNode_t = UDSelfListNode<Buffer_t>;
    using BufferList_t = UDSelfList<BufferNode_t>;
    using BufferPool_t = ObjectPool<BufferNode_t, StepAllocator<256>, std::mutex>;

    using TraverseCallback_t = std::function<bool(BufferNode_t* buffer)>;

private:
    BufferList_t m_bufferList;
    BufferPool_t* m_bufferPool = nullptr;
    int m_stepSize = 0;
    int m_capLength = 0;
    int m_curLength = 0;

public:
    OutputBuffer() {}

    ~OutputBuffer() { Reset(); }

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
        m_stepSize = 0;
        m_capLength = 0;
        m_curLength = 0;
    }

    void Attach(BufferNode_t* buffer) {
        _Attach(buffer);
    }

    void Traverse(TraverseCallback_t callback) {
        auto buffer = m_bufferList.Front();
        while (!IsNull(buffer)) {
            const auto result = callback(buffer);
            if (result) {
                break;
            }

            buffer = m_bufferList.Forward(buffer);
        }
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

    int GetMaxSize() const {
        return MAXSIZE;
    }

    int GetStepSize() const {
        return m_stepSize;
    }

    int GetMaxLength() const {
        return MAXLENGTH;
    }

    int GetCapLength() const {
        return m_capLength;
    }

    int GetPushLength() const {
        return m_capLength - m_curLength;
    }

    int GetPullLength() const {
        return m_curLength;
    }

    bool IsEmpty() const {
        return m_curLength == 0;
    }

    bool IsFull() const {
        return m_curLength == m_capLength;
    }

private:
    void _Attach(BufferNode_t* buffer) {
        if (m_stepSize < MAXSIZE) {
            const auto pullLength = buffer->m_value.GetPullLength();
            m_bufferList.PushBack(buffer);
            ++m_stepSize;
            m_capLength += buffer->m_value.GetMaxLength();
            m_curLength += pullLength;
        }
    }

    int _Pull(BufferNode_t* buffer, void* data, int length) {
        if (IsNull(buffer)) {
            return 0;
        }

        const auto pullLength = buffer->m_value.Pull(data, length);
        m_curLength -= pullLength;
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
        m_curLength -= skipLength;
        return skipLength;
    }

    void Compact() {
        auto buffer = m_bufferList.Front();
        if (IsNull(buffer)) {
            return;
        }

        if (buffer->m_value.GetPullLength() > 0) {
            return;
        }

        m_bufferList.PopFront();
        m_bufferPool->Return(buffer);
        --m_stepSize;
        m_capLength -= buffer->m_value.GetMaxLength();
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_NET_OUTPUTBUFFER_HPP_
