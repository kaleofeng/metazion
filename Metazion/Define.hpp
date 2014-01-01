#ifndef MZ_DEFINE_HPP
#define MZ_DEFINE_HPP

#include "Metazion/CoreInclude.hpp"

#include <cassert>

DECL_NAMESPACE_MZ_BEGIN

#define DECL_BLOCK_BEGIN    {
#define DECL_BLOCK_END      }

#define ASSERT_TRUE(exp)    assert( (exp) )
#define ASSERT_FALSE(exp)   assert( !(exp) )

#define RELEASE_ASSERT_TRUE(exp)    do { if (!(exp)) __asm { int 3 } } while (false)
#define RELEASE_ASSERT_FALSE(exp)   do { if ((exp)) __asm { int 3 } } while (false)

#define DISALLOW_COPY_AND_ASSIGN(CLASS) \
    CLASS(const CLASS&); \
    CLASS& operator =(const CLASS&);

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define DEFAULT_ALIGNMENT (sizeof(void*))
#define ALIGN_LENGTH(length, align) (((length) + (align) - 1) & ~((align) - 1))
#define IS_LENGTH_ALIGN(length, align) (0 == ((length) & ((align) -1)))

DECL_NAMESPACE_MZ_END

#endif // MZ_DEFINE_HPP
