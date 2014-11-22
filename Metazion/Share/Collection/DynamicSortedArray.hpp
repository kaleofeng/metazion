#ifndef _MZ_SHARE_DYNAMICSORTEDARRAY_HPP_
#define _MZ_SHARE_DYNAMICSORTEDARRAY_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/DynamicArray.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType
, typename CompareType = LessCompare<ValueType>
, int INITSIZE = 16
, int STEPSIZE = 16
>
class DynamicSortedArray {
    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Array_t = DynamicArray<Value_t, INITSIZE, STEPSIZE>;

public:
    DynamicSortedArray() {}

    ~DynamicSortedArray() {}

    DynamicSortedArray(const DynamicSortedArray& other)
        : DynamicSortedArray() {
        *this = other;
    }

    DynamicSortedArray& operator =(const DynamicSortedArray& other) {
        if (&other != this) {
            Clear();
            m_array = other.m_array;
        }
        return *this;
    }

    DynamicSortedArray(DynamicSortedArray&& other)
        : DynamicSortedArray() {
        *this = std::move(other);
    }

    DynamicSortedArray& operator =(DynamicSortedArray&& other) {
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

    Value_t& At(int index) {
        return m_array.At(index);
    }

    const Value_t& At(int index) const {
        return m_array.At(index);
    }

    Value_t& operator [](int index) {
        return m_array[index];
    }

    const Value_t& operator [](int index) const {
        return m_array[index];
    }

    int Find(const Value_t& value) const {
        int index = 0;
        auto found = BinarySearch(value, index);
        return found ? index : -1;
    }

    int Add(const Value_t& value) {
        int index = 0;
        auto found = BinarySearch(value, index);
        return m_array.Insert(index, value);
    }

    int Remove(int index) {
        return m_array.Remove(index);
    }

    int GetCapacity() const {
        return m_array.GetCapacity();
    }

    int GetSize() const {
        return m_array.GetSize();
    }

private:
    bool BinarySearch(const Value_t& value, int& index) const {
        auto low = 0;
        auto high = m_array.GetSize() - 1;
        auto middle = 0;
        auto found = false;
        while (low <= high) {
            middle = low + ((high - low) >> 1);
            const auto ret = m_compare(value, m_array[middle]);
            if (ret > 0) {
                low = middle + 1;
            }
            else if (ret < 0) {
                high = middle - 1;
            }
            else {
                low = middle;
                found = true;
                break;
            }
        }

        index = low;
        return found;
    }

private:
    Compare_t m_compare;
    Array_t m_array;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_DYNAMICSORTEDARRAY_HPP_
