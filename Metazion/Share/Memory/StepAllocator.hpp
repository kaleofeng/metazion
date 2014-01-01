#ifndef MZ_SHARE_STEPALLOCATOR_HPP
#define MZ_SHARE_STEPALLOCATOR_HPP

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Memory/MemoryPiece.hpp"
#include "Metazion/Share/Collection/UnidSelfList.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int STEPSIZE
, int UNITLENGTH = DEFAULT_ALIGNMENT
>
class StepAllocator {
    DISALLOW_COPY_AND_ASSIGN(StepAllocator)

    typedef UnidSelfListNode<void> UnitNode_t;
    typedef UnidSelfList<UnitNode_t> UnitList_t;

    enum {
        MINLENGTH = MAX(UNITLENGTH, sizeof(UnitNode_t)),
        ALIGNLENGTH = ALIGN_LENGTH(MINLENGTH, DEFAULT_ALIGNMENT),
    };

    typedef MemoryPiece<STEPSIZE, ALIGNLENGTH> Piece_t;

    typedef UnidSelfListNode<Piece_t> PieceNode_t;
    typedef UnidSelfList<PieceNode_t> PieceList_t;

public:
    template<int OTHERLENGTH>
    struct Rebind{
        typedef StepAllocator<STEPSIZE, OTHERLENGTH> Allocator_t;
    };

public:
    StepAllocator() {}

    ~StepAllocator() {}

public:
    void Initialize() {}

    void Finalize() {
       m_freeUnitList.Clear();
       m_fullPieceList.Clear();
       m_availPieceList.Clear();
    }

    void* Alloc() {
        UnitNode_t* unitNode = m_freeUnitList.Front();
        if (!IsNull(unitNode)) {
            m_freeUnitList.PopFront();
            return unitNode;
        }

        PieceNode_t* pieceNode = m_availPieceList.Front();
        if (IsNull(pieceNode)) {
            pieceNode = CreatePieceNode();
            m_availPieceList.PushBack(pieceNode);
        }

        Piece_t& piece = pieceNode->m_value;
        void* memory = piece.Obtain();
        if (!piece.IsAvaliable()) {
            m_availPieceList.PopFront();
            m_fullPieceList.PushBack(pieceNode);
        }

        return memory;
    }

    void Free(void* memory) {
        UnitNode_t* unitNode = static_cast<UnitNode_t*>(memory);
        m_freeUnitList.PushBack(unitNode);
    }

private:
    PieceNode_t* CreatePieceNode() {
        return new PieceNode_t();
    }

    void DestoryPieceNode(PieceNode_t* object) {
        SafeDelete(object);
    }

private:
    PieceList_t m_fullPieceList;
    PieceList_t m_availPieceList;
    UnitList_t m_freeUnitList;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_STEPALLOCATOR_HPP
