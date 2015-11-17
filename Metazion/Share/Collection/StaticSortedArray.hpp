#ifndef _MZ_SHARE_STATICSORTEDARRAY_HPP_
#define _MZ_SHARE_STATICSORTEDARRAY_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/StaticArray.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType
, typename CompareType = LessCompare<ValueType>
>
class StaticSortedArray {
    MZ_DISALLOW_COPY_AND_ASSIGN(StaticSortedArray);

    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Array_t = StaticArray<Value_t>;

private:
    Compare_t m_compare;
    Array_t m_array;

public:
    StaticSortedArray() {}

    ~StaticSortedArray() {}

public:
    void Attach(Value_t* buffer, int capacity, int size) {
        m_array.Attach(buffer, capacity, size);
    }

    void Detach() {
        m_array.Detach();
    }

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

    int Find(const Value_t& value) {
        int index = 0;
        auto found = BinarySearch(value, index);
        return found ? index : -1;
    }

    int Append(const Value_t& value) {
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
    bool BinarySearch(const Value_t& value, int& index) {
        int low = 0;
        int high = m_array.GetSize() - 1;
        int middle = 0;
        bool found = false;
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
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_STATICSORTEDARRAY_HPP_
