#ifndef _MZ_NET_INPUTBUFFER_HPP_
#define _MZ_NET_INPUTBUFFER_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <mutex>

#include "Metazion/Share/Collection/UDSelfList.hpp"
#include "Metazion/Share/Memory/PieceBuffer.hpp"
#include "Metazion/Share/Memory/ObjectPool.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int STEPLENGTH
, int MAXSIZE
>
class InputBuffer {
    MZ_DISALLOW_COPY_AND_ASSIGN(InputBuffer)

    enum { MAXLENGTH = STEPLENGTH * MAXSIZE };

public:
    using Buffer_t = PieceBuffer<STEPLENGTH>;
    using BufferNode_t = UDSelfListNode<Buffer_t>;
    using BufferList_t = UDSelfList<BufferNode_t>;
    using BufferPool_t = ObjectPool<BufferNode_t, StepAllocator<1024>, std::mutex>;

    using TraverseCallback_t = std::function<bool(BufferNode_t* buffer)>;

private:
    BufferList_t m_bufferList;
    BufferPool_t* m_bufferPool{ nullptr };
    int m_stepSize{ 0 };
    int m_capLength{ 0 };
    int m_curLength{ 0 };

public:
    InputBuffer() {}

    ~InputBuffer() { Reset(); }

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

    BufferNode_t* Detach() {
        return _Detach();
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

    int Push(const void* data, int length) {
        auto tData = static_cast<const char*>(data);
        auto tLength = length;
        while (tLength > 0) {
            auto buffer = m_bufferList.Back();
            const int pushLength = _Push(buffer, tData, tLength);
            if (pushLength <= 0) {
                if (!_Inflate()) {
                    break;
                }
            }
            tData += pushLength;
            tLength -= pushLength;
        }
        return length - tLength;
    }

    int Take(int length) {
        auto tLength = length;
        auto buffer = m_bufferList.Front();
        while (tLength > 0) {
            const auto takeLength = _Take(buffer, tLength);
            if (takeLength <= 0) {
                break;
            }
            tLength -= takeLength;
            buffer = m_bufferList.Forward(buffer);
        }
        return length - tLength;
    }

    int Expand(int length) {
        auto tLength = length;
        while (tLength > 0) {
            if (!_Inflate()) {
                break;
            }
            tLength -= STEPLENGTH;
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

    int GetCurLength() const {
        return m_curLength;
    }

    bool IsMaximal() const {
        return m_stepSize == MAXSIZE;
    }

    bool IsEmpty() const {
        return m_curLength == 0;
    }

    bool IsFull() const {
        return m_curLength == m_capLength;
    }

private:
    BufferNode_t* _Detach() {
        auto buffer = m_bufferList.Front();
        if (!IsNull(buffer)) {
            const auto pullLength = buffer->m_value.GetPullLength();
            m_bufferList.PopFront();
            --m_stepSize;
            m_capLength -= buffer->m_value.GetMaxLength();
            m_curLength -= pullLength;
        }
        return buffer;
    }

    int _Push(BufferNode_t* buffer, const void* data, int length) {
        if (IsNull(buffer)) {
            return 0;
        }

        const auto pushLength = buffer->m_value.Push(data, length);
        m_curLength += pushLength;
        return pushLength;
    }

    int _Take(BufferNode_t* buffer, int length) {
        if (IsNull(buffer)) {
            return 0;
        }

        const auto takeLength = buffer->m_value.Take(length);
        m_curLength += takeLength;
        return takeLength;
    }

    bool _Inflate() {
        if (m_stepSize >= MAXSIZE) {
            return false;
        }

        auto buffer = m_bufferPool->Obtain();
        if (IsNull(buffer)) {
            return false;
        }

        buffer->m_value.Reset();
        m_bufferList.PushBack(buffer);
        ++m_stepSize;
        m_capLength += buffer->m_value.GetMaxLength();
        return true;
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_NET_INPUTBUFFER_HPP_
