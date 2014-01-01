#ifndef MZ_SHARE_RANDOM_HPP
#define MZ_SHARE_RANDOM_HPP

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

#endif // MZ_SHARE_UTILITY_HPP
