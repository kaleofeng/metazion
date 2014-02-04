#ifndef _MZ_SHARE_BLOCKALLOCATOR_HPP_
#define _MZ_SHARE_BLOCKALLOCATOR_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Memory/MemoryPiece.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int MAXSIZE
, int UNITLENGTH = DEFAULT_ALIGNMENT
>
class BlockAllocator {
    DISALLOW_COPY_AND_ASSIGN(BlockAllocator)

    enum { ALIGNLENGTH = ALIGN_LENGTH(UNITLENGTH, DEFAULT_ALIGNMENT) };

    using Piece_t = MemoryPiece<MAXSIZE, ALIGNLENGTH>;

public:
    template<int OTHERLENGTH>
    using Rebind = BlockAllocator<MAXSIZE, OTHERLENGTH>;

public:
    BlockAllocator()
        : m_piece(nullptr) {}

    ~BlockAllocator() {}

public:
    void Initialize() {
        m_piece = new Piece_t();
    }

    void Finalize() {
        SafeDelete(m_piece);
    }

    void* Alloc() {
        return m_piece->Obtain();
    }

    void Free(void* memory) {
        return m_piece->Return(memory);
    }

private:
    Piece_t* m_piece;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_BLOCKALLOCATOR_HPP_
