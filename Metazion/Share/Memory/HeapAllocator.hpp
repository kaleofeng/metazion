#ifndef MZ_SHARE_HEAPALLOCATOR_HPP
#define MZ_SHARE_HEAPALLOCATOR_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int UNITLENGTH = DEFAULT_ALIGNMENT>
class HeapAllocator {
    DISALLOW_COPY_AND_ASSIGN(HeapAllocator)

    enum { ALIGNLENGTH = ALIGN_LENGTH(UNITLENGTH, DEFAULT_ALIGNMENT) };

public:
    template<int OTHERLENGTH>
    using Rebind = HeapAllocator<OTHERLENGTH>;

public:
    HeapAllocator() {}

    ~HeapAllocator() {}

public:
    void Initialize() {}

    void Finalize() {}

    void* Alloc() {
        return malloc(ALIGNLENGTH);
    }

    void Free(void* memory) {
        free(memory);
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_HEAPALLOCATOR_HPP
