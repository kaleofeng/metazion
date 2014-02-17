#ifndef _MZ_SHARE_DYNAMICARRAY_HPP_
#define _MZ_SHARE_DYNAMICARRAY_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType
, int INITSIZE = 16
, int STEPSIZE = 16
>
class DynamicArray {
    using Value_t = ValueType;

public:
    DynamicArray()
        : m_values(nullptr)
        , m_capacity(0)
        , m_size(0) {}

    ~DynamicArray() { 
        Clear();
    }

    DynamicArray(DynamicArray& other)
        : DynamicArray() {
        *this = other;
    }

    DynamicArray& operator =(DynamicArray& other) {
        if (&other != this) {
            Clear();
            m_values = new Value_t[other.m_capacity];
            ::memcpy(m_values, other.m_values, sizeof(Value_t) * other.m_size);
            m_capacity = other.m_capacity;
            m_size = other.m_size;
        }

        return *this;
    }

    DynamicArray(DynamicArray&& other)
        : DynamicArray() {
        *this = std::move(other);
    }

    DynamicArray& operator =(DynamicArray&& other) {
        if (&other != this) {
            m_values = other.m_values;
            m_capacity = other.m_capacity;
            m_size = other.m_size;

            other.m_values = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }

        return *this;
    }

public:
    void Clear() {
        SafeDeleteArray(m_values);
        m_capacity = 0;
        m_size = 0;
    }

    Value_t& At(int index) {
        ASSERT_TRUE(index >= 0 && index < m_size);

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

    int Insert(int index, const Value_t& value) {
        if (index >= m_size) {
            return Set(index, value);
        }

        if (m_size >= m_capacity) {
            Inflate(m_size + 1);
        }

        ::memmove(m_values + index + 1, m_values + index
            , sizeof(Value_t)* (m_size - index));

        m_values[index] = value;
        ++m_size;
        return index;
    }

    int Add(const Value_t& value) {
        const int index = m_size;
        return Set(index, value);
    }

    int Remove(int index) {
        if (index < 0 || index >= m_size) {
            return -1;
        }

        ::memmove(m_values + index, m_values + index + 1
            , sizeof(Value_t) * (m_size - index - 1));
        --m_size;
        return index;
    }

    int GetCapacity() const {
        return m_capacity;
    }

    int GetSize() const {
        return m_size;
    }

private:
    int Set(int index, const Value_t& value) {
        ASSERT_TRUE(index >= 0);

        const int size = index + 1;
        if (size > m_capacity) {
            Inflate(size);
        }

        if (size > m_size) {
            m_size = size;
        }

        m_values[index] = value;
        return index;
    }

    void Inflate(int capacity) {
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
        ::memmove(tValues, m_values, sizeof(Value_t) * m_size);
        SafeDeleteArray(m_values);
        m_values = tValues;
    }

private:
    Value_t* m_values;
    int m_capacity;
    int m_size;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_DYNAMICARRAY_HPP_
