#ifndef MZ_SHARE_DYNAMICARRAY_HPP
#define MZ_SHARE_DYNAMICARRAY_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType
, int INITSIZE = 16
, int STEPSIZE = 16
>
class DynamicArray {
    DISALLOW_COPY_AND_ASSIGN(DynamicArray)

    typedef ValueType Value_t;

public:
    DynamicArray()
        : m_values(nullptr)
        , m_capacity(0)
        , m_size(0)
        , m_dependent(false) {}

    ~DynamicArray() { Clear(); }

public:
    void Clear() {
        if (!m_dependent) {
            SafeDeleteArray(m_values);
            m_capacity = 0;
        }
        m_size = 0;
    }

    void Attach(Value_t* buffer, int capacity, int size) {
        Clear();
        m_values = buffer;
        m_capacity = capacity;
        m_size = size;
        m_dependent = true;
    }

    void Detach() {
        if (m_dependent) {
            m_values = nullptr;
            m_capacity = 0;
            m_size = 0;
            m_dependent = false;
        }
    }

    Value_t& At(int index) {
        ASSERT_TRUE(index >= 0);
        if (index >= m_capacity) {
            Inflate(index + 1);
        }
        return m_values[index];
    }

    const Value_t& At(int index) const {
        ASSERT_TRUE(index >= 0 && index < m_size);
        return m_values[index];
    }

    Value_t& operator [](int index) {
        return At(index);
    }

    const Value_t& operator [](int index) const {
        return At(index);
    }

    int Add(const Value_t& element) {
        if (m_size >= m_capacity) {
            Inflate(m_size + 1);
        }
        const int index = m_size;
        m_values[index] = element;
        ++m_size;
        return index;
    }

    int Insert(int index, const Value_t& element) {
        bool needInflate = true;
        int capacity = 0;
        if (index >= m_capacity) {
            capacity = index + 1;
        }
        else if (m_size >= m_capacity) {
            capacity = m_size;
        }
        else {
            needInflate = false;
        }

        if (needInflate) {
            Inflate(capacity);
        }

        if (index < m_size) {
            memmove(m_values + index + 1, m_values + index
                , sizeof(Value_t) * (m_size - index));
        }
        m_values[index] = element;
        ++m_size;
        return index;
    }

    int Remove(int index) {
        if (index < 0 || index >= m_size) {
            return -1;
        }
        memmove(m_values + index, m_values + index + 1
            , sizeof(Value_t) * (m_size - index - 1));
        --m_size;
        return index;
    }

    int GetCapacity() { return m_capacity; }

    int GetSize() { return m_size; }

private:
    void Inflate(int capacity) {
        ASSERT_TRUE(!m_dependent);
        ASSERT_TRUE(capacity > m_capacity);
        
        if (m_capacity <= 0) {
            m_capacity = INITSIZE;
        }

        while (m_capacity < capacity) {
            m_capacity += STEPSIZE;
        }

        if (IsNull(m_values)) {
            m_values = new Value_t[m_capacity];
            return;
        }

        Value_t* tValues = new Value_t[m_capacity];
        memmove(tValues, m_values, sizeof(Value_t) * m_size);
        SafeDeleteArray(m_values);
        m_values = tValues;
    }

private:
    Value_t* m_values;
    int m_capacity;
    int m_size;
    bool m_dependent;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_DYNAMICARRAY_HPP
