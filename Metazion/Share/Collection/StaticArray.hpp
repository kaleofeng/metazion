#ifndef _MZ_SHARE_STATICARRAY_HPP_
#define _MZ_SHARE_STATICARRAY_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType>
class StaticArray {
    MZ_DISALLOW_COPY_AND_ASSIGN(StaticArray);

    using Value_t = ValueType;

private:
    Value_t* m_values = nullptr;
    int m_capacity = 0;
    int m_size = 0;

public:
    StaticArray() {}

    ~StaticArray() {}

public:
    void Attach(Value_t* buffer, int capacity, int size) {
        m_values = buffer;
        m_capacity = capacity;
        m_size = size;
    }

    void Detach() {
        m_values = nullptr;
        m_capacity = 0;
        m_size = 0;
    }

    void Clear() {
        m_size = 0;
    }

    Value_t& At(int index) {
        MZ_ASSERT_TRUE(index >= 0 && index < m_size);

        return m_values[index];
    }

    const Value_t& At(int index) const {
        MZ_ASSERT_TRUE(index >= 0 && index < m_size);

        return m_values[index];
    }

    Value_t& operator [](int index) {
        return At(index);
    }

    const Value_t& operator [](int index) const {
        return At(index);
    }

    int Insert(int index, const Value_t& value) {
        if (m_size >= m_capacity || index >= m_capacity) {
            return -1;
        }

        if (index >= m_size) {
            return Set(index, value);
        }

        memmove(m_values + index + 1, m_values + index
            , sizeof(Value_t)* (m_size - index));

        m_values[index] = value;
        ++m_size;
        return index;
    }

    int Append(const Value_t& value) {
        if (m_size >= m_capacity) {
            return -1;
        }

        const int index = m_size;
        return Set(index, value);
    }

    int Remove(int index) {
        if (index < 0 || index >= m_size) {
            return -1;
        }

        memmove(m_values + index, m_values + index + 1
            , sizeof(Value_t)* (m_size - index - 1));
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
        MZ_ASSERT_TRUE(index >= 0 && index < m_capacity);

        const auto size = index + 1;
        if (size > m_size) {
            m_size = size;
        }

        m_values[index] = value;
        return index;
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_STATICARRAY_HPP_
