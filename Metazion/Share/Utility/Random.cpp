#include "Metazion/Share/Utility/Random.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

Random::Random() {
    m_randomEngine = std::mt19937(m_randomDevice());
}

Random::~Random() {}

int Random::GetRangeInt(int begin, int end) {
    MZ_ASSERT_TRUE(begin <= end);

    if (begin >= end) {
        return begin;
    }

    std::uniform_int_distribution<int> distribution(begin, end);
    return distribution(m_randomEngine);
}

double Random::GetRangeDouble(double begin, double end) {
    MZ_ASSERT_TRUE(begin <= end);

    if (begin >= end) {
        return begin;
    }

    std::uniform_real_distribution<double> distribution(begin, end);
    return distribution(m_randomEngine);
}

DECL_NAMESPACE_MZ_SHARE_END
