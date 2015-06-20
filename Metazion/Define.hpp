#ifndef _MZ_DEFINE_HPP_
#define _MZ_DEFINE_HPP_

#include "Metazion/CoreInclude.hpp"

#include <cassert>

DECL_NAMESPACE_MZ_BEGIN

#if defined(MZ_PLATFORM_MACOS)
#   define thread_local
#endif

#if defined(MZ_MODE_DEBUG)
#   define MZ_ENABLE_STATISTIC
#endif

#define DECL_BLOCK_BEGIN    {
#define DECL_BLOCK_END      }

#define MZ_DISALLOW_COPY_AND_ASSIGN(CLASS) \
    CLASS(const CLASS&) = delete; \
    CLASS& operator =(const CLASS&) = delete; \
    CLASS(const CLASS&&) = delete; \
    CLASS& operator =(const CLASS&&) = delete;

#define MZ_ADAPT_FOR_RANGE_TRAVERSAL() \
    ConstIterator_t begin() const { \
        return Begin(); \
    } \
    Iterator_t begin() { \
        return Begin(); \
    } \
    ConstIterator_t end() const { \
        return End(); \
    } \
    Iterator_t end() { \
        return End(); \
    }

#define MZ_ASSERT_TRUE(exp)    assert( (exp) )
#define MZ_ASSERT_FALSE(exp)   assert( !(exp) )

#define MZ_RELEASE_ASSERT_TRUE(exp)    do { if (!(exp)) __asm { int 3 } } while (false)
#define MZ_RELEASE_ASSERT_FALSE(exp)   do { if ((exp)) __asm { int 3 } } while (false)

#define MZ_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MZ_MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MZ_DEFAULT_ALIGNMENT (sizeof(void*))
#define MZ_ALIGN_LENGTH(length, align) (((length) + (align) - 1) & ~((align) - 1))
#define MZ_IS_LENGTH_ALIGN(length, align) (((length) & ((align) -1)) == 0)

DECL_NAMESPACE_MZ_END

#endif // _MZ_DEFINE_HPP_
