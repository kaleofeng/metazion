#ifndef _MZ_SHARE_BLOCKALLOCATOR_HPP_
#define _MZ_SHARE_BLOCKALLOCATOR_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Memory/MemoryPiece.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int MAXSIZE
, int UNITLENGTH = MZ_DEFAULT_ALIGNMENT
>
class BlockAllocator {
    MZ_DISALLOW_COPY_AND_ASSIGN(BlockAllocator)

    enum { ALIGNLENGTH = MZ_ALIGN_LENGTH(UNITLENGTH, MZ_DEFAULT_ALIGNMENT) };

    using Piece_t = MemoryPiece<MAXSIZE, ALIGNLENGTH>;

public:
    template<int OTHERLENGTH>
    using Rebind = BlockAllocator<MAXSIZE, OTHERLENGTH>;

private:
    Piece_t* m_piece = nullptr;

public:
    BlockAllocator() {
        m_piece = new Piece_t();
    }

    ~BlockAllocator() {
        SafeDelete(m_piece);
    }

public:
    void* Alloc() {
        return m_piece->Obtain();
    }

    void Free(void* memory) {
        return m_piece->Return(memory);
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_BLOCKALLOCATOR_HPP_
