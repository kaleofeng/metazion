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
    DISALLOW_COPY_AND_ASSIGN(PriorityQueue)

    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Array_t = DynamicArray<Value_t, INITSIZE + 1, STEPSIZE>;

public:
    PriorityQueue() {
        m_array.Add(Value_t());
    }

    ~PriorityQueue() {}

public:
    void Clear() {
        m_array.Clear();
        m_array.Add(Value_t());
    }

    const Value_t& Top() const {
        ASSERT_TRUE(GetSize() > 0);

        return m_array[1];
    }

    Value_t& Top() {
        ASSERT_TRUE(GetSize() > 0);

        return m_array[1];
    }

    void Push(const Value_t& value) {
        m_array.Add(value);

        const auto size = GetSize();
        SiftUp(size);
    }

    void Pop() {
        const auto size = GetSize();
        ASSERT_TRUE(size > 0);

        m_array[1] = m_array[size];
        m_array.Remove(size);
        SiftDown(1);
    }

    int GetSize() const {
        return m_array.GetSize() - 1;
    }

    bool IsEmpty() const {
        return GetSize() == 0;
    }

private:
    void SiftUp(int index) {
        while (index > 1) {
            const auto parent = index >> 1;
            if (m_compare(m_array[parent], m_array[index]) < 0) {
                break;
            }

            const auto temp = m_array[index];
            m_array[index] = m_array[parent];
            m_array[parent] = temp;

            index = parent;
        }
    }

    void SiftDown(int index){
        const auto size = GetSize();
        auto next = index << 1;
        while (next <= size) {
            const auto right = next + 1;
            if (right <= size && m_compare(m_array[right], m_array[next]) < 0) {
                next = right;
            }

            if (m_compare(m_array[index], m_array[next]) < 0) {
                break;
            }
            
            const auto temp = m_array[next];
            m_array[next] = m_array[index];
            m_array[index] = temp;

            index = next;
            next = index << 1;
        }
    }

private:
    Array_t m_array;
    Compare_t m_compare;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_PRIORITY_HPP_
