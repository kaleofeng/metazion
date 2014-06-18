#ifndef _MZ_SHARE_RANDOM_HPP_
#define _MZ_SHARE_RANDOM_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <random>

DECL_NAMESPACE_MZ_SHARE_BEGIN

class Random  {
    DISALLOW_COPY_AND_ASSIGN(Random)

    using RandomDevice_t = std::random_device;
    using RandomEngine_t = std::mt19937;

public:
    Random();

    ~Random();

public:
    // value range: [begin, end]
    int GetRangeInt(int begin, int end);

    // value range: [begin, end)
    double GetRangeDouble(double begin, double end);

private:
    RandomDevice_t m_randomDevice;
    RandomEngine_t m_randomEngine;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_UTILITY_HPP_
