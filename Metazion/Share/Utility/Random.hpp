#ifndef _MZ_SHARE_RANDOM_HPP_
#define _MZ_SHARE_RANDOM_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class Random  {
public:
    Random();

    ~Random();

public:
    void Seed(uint32_t seed);

    int GetRandInt();

    int GetRangeInt(int begin, int end);

    double GetRandRate();

private:
    uint32_t m_seed;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_UTILITY_HPP_
