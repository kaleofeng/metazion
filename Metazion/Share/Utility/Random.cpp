#include "Metazion/Share/Utility/Random.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

Random::Random()
    : m_seed(0) {
    Seed(static_cast<uint32_t>(time(nullptr)));
}

Random::~Random() {}

void Random::Seed(uint32_t seed) {
    m_seed = seed;
}

int Random::GetRandInt() {
    m_seed = ((m_seed * 214013L + 2531011L) >> 16) & 0x7fff;
    return m_seed;
}

int Random::GetRangeInt(int begin, int end) {
    if (begin == end) {
        return begin;
    }
	
    return begin + GetRandInt() % (end - begin + 1);
}

double Random::GetRandRate() {
    return GetRandInt() / static_cast<double>(RAND_MAX);
}

DECL_NAMESPACE_MZ_SHARE_END
