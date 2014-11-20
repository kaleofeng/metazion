#ifndef _MZ_SHARE_PRIORITY_HPP_
#define _MZ_SHARE_PRIORITY_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/DynamicArray.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType
, typename CompareType = LessCompare<ValueType>
, int INITSIZE = 16
, int STEPSIZE = 16
>
class PriorityQueue {
    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Array_t = DynamicArray<Value_t, INITSIZE, STEPSIZE>;

public:
    PriorityQueue() {}

    ~PriorityQueue() {}

    PriorityQueue(PriorityQueue& other)
        : PriorityQueue() {
        *this = other;
    }

    PriorityQueue& operator =(PriorityQueue& other) {
        if (&other != this) {
            Clear();
            m_array = other.m_array;
        }
        return *this;
    }

    PriorityQueue(PriorityQueue&& other)
        : PriorityQueue() {
        *this = std::move(other);
    }

    PriorityQueue& operator =(PriorityQueue&& other) {
        if (&other != this) {
            Clear();
            m_array = std::move(other.m_array);
        }
        return *this;
    }

public:
    void Clear() {
        m_array.Clear();
    }

    const Value_t& Top() const {
        ASSERT_TRUE(GetSize() > 0);

        return m_array[0];
    }

    Value_t& Top() {
        ASSERT_TRUE(GetSize() > 0);

        return m_array[0];
    }

    void Push(const Value_t& value) {
        m_array.Add(value);

        const auto size = GetSize();
        SiftUp(size - 1);
    }

    void Pop() {
        const auto size = GetSize();
        ASSERT_TRUE(size > 0);
        m_array[0] = m_array[size - 1];
        m_array.Remove(size - 1);

        SiftDown(0);
    }

    int GetSize() const {
        return m_array.GetSize();
    }

    bool IsEmpty() const {
        return GetSize() == 0;
    }

private:
    void SiftUp(int localIndex) {
        auto index = ToLogicIndex(localIndex);
        while (index > 1) {
            const auto parent = index >> 1;
            if (m_compare(Data(parent), Data(index)) < 0) {
                break;
            }

            std::swap(Data(index), Data(parent));

            index = parent;
        }
    }

    void SiftDown(int localIndex){
        auto index = ToLogicIndex(localIndex);
        const auto size = GetSize();
        auto next = index << 1;
        while (next <= size) {
            const auto right = next + 1;
            if (right <= size && m_compare(Data(right), Data(next)) < 0) {
                next = right;
            }

            if (m_compare(Data(index), Data(next)) < 0) {
                break;
            }

            std::swap(Data(next), Data(index));

            index = next;
            next = index << 1;
        }
    }

    Value_t& Data(int logicIndex) {
        return m_array[ToLocalIndex(logicIndex)];
    }

    int ToLogicIndex(int localIndex) const {
        return localIndex + 1;
    }

    int ToLocalIndex(int logicIndex) const {
        return logicIndex - 1;
    }

private:
    Compare_t m_compare;
    Array_t m_array;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_PRIORITY_HPP_
