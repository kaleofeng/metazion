#ifndef _MZ_TYPE_HPP_
#define _MZ_TYPE_HPP_

#include "Metazion/CoreInclude.hpp"

DECL_NAMESPACE_MZ_BEGIN

template<typename T>
struct LessCompare {
    int operator ()(const T& left, const T& right) const {
        if (left < right) {
            return -1;
        }
        else if (left > right) {
            return 1;
        }
        else {
            return 0;
        }
    }
};

template<typename T>
struct GreaterCompare {
    int operator ()(const T& left, const T& right) const {
        if (left > right) {
            return -1;
        }
        else if (left < right) {
            return 1;
        }
        else {
            return 0;
        }
    }
};

template<typename FirstType
, typename SecondType
>
struct Pair {
    using First_t = FirstType;
    using Second_t = SecondType;

    First_t first;
    Second_t second;

    Pair() {}

    Pair(const First_t& first, const Second_t& second)
        : first(first)
        , second(second) {}

    Pair(const Pair& other)
        : first(other.first)
        , second(other.second) {}

    Pair& operator =(const Pair& other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }
};

template<typename PairType
, typename CompareType = LessCompare<PairType>
>
struct PairCompare {
    using Pair_t = PairType;
    using Compare_t = CompareType;

    Compare_t m_compare;

    int operator ()(const Pair_t& left, const Pair_t& right) const {
        return m_compare(left.first, right.first);
    }
};

DECL_NAMESPACE_MZ_END

#endif // _MZ_TYPE_HPP_
