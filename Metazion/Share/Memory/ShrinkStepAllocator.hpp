#ifndef _MZ_SHARE_SHRINKSTEPALLOCATOR_HPP_
#define _MZ_SHARE_SHRINKSTEPALLOCATOR_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/UDSelfList.hpp"
#include "Metazion/Share/Memory/MemoryPiece.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<int STEPSIZE
, int UNITLENGTH = MZ_DEFAULT_ALIGNMENT
>
class ShrinkStepAllocator {
    MZ_DISALLOW_COPY_AND_ASSIGN(ShrinkStepAllocator)

    using UnitNode_t = UDSelfListNode<void>;
    using UnitList_t = UDSelfList<UnitNode_t>;

    enum {
        MINLENGTH = MZ_MAX(UNITLENGTH, sizeof(UnitNode_t)),
        ALIGNLENGTH = MZ_ALIGN_LENGTH(MINLENGTH, MZ_DEFAULT_ALIGNMENT),
    };

    using Piece_t = MemoryPiece<STEPSIZE, ALIGNLENGTH>;
    using PieceNode_t = UDSelfListNode<Piece_t>;
    using PieceList_t = UDSelfList<PieceNode_t>;

public:
    template<int OTHERLENGTH>
    using Rebind = ShrinkStepAllocator<STEPSIZE, OTHERLENGTH>;

public:
    ShrinkStepAllocator() {}

    ~ShrinkStepAllocator() {
        m_freeUnitList.Clear();
        m_fullPieceList.Clear();
        m_availPieceList.Clear();
    }

public:
    void* Alloc() {
        auto unitNode = m_freeUnitList.Front();
        if (!IsNull(unitNode)) {
            m_freeUnitList.PopFront();
            return unitNode;
        }

        auto pieceNode = m_availPieceList.Front();
        if (IsNull(pieceNode)) {
            pieceNode = CreatePieceNode();
            m_availPieceList.PushBack(pieceNode);
        }

        auto& piece = pieceNode->m_value;
        auto memory = piece.Obtain();
        if (!piece.IsAvaliable()) {
            m_availPieceList.PopFront();
            m_fullPieceList.PushBack(pieceNode);
        }

        return memory;
    }

    void Free(void* memory) {
        auto unitNode = static_cast<UnitNode_t*>(memory);
        m_freeUnitList.PushBack(unitNode);
        if (ShouldShrink()) {
            Shrink();
        }
    }

private:
    PieceNode_t* CreatePieceNode() {
        return new PieceNode_t();
    }

    void DestoryPieceNode(PieceNode_t* object) {
        SafeDelete(object);
    }

    int GetPieceCount() const {
        return m_fullPieceList.GetSize() + m_availPieceList.GetSize();
    }

    bool ShouldShrink() {
        const auto pieceCount = GetPieceCount();
        if (pieceCount < 4) {
            return false;
        }

        const auto freeUnitCount = m_freeUnitList.GetSize();
        const auto freePieceCount = freeUnitCount / STEPSIZE;
        if (freePieceCount * 2 < pieceCount) {
            return false;
        }

        return true;
    }

    void Shrink() {
        int pieceCount = GetPieceCount();

        PieceList_t tempPieceList;
        PieceList_t emptyPieceList;

        tempPieceList.Clear();
        auto fullPiece = m_fullPieceList.Front();
        while (!IsNull(fullPiece)) {
            m_fullPieceList.PopFront();
            ShrinkPiece(fullPiece);
            auto& piece = fullPiece->m_value;
            if (piece.IsEmpty()) {
                emptyPieceList.PushBack(fullPiece);
            }
            else if (piece.IsAvaliable()) {
                m_availPieceList.PushBack(fullPiece);
            }
            else {
                tempPieceList.PushBack(fullPiece);
            }
            fullPiece = m_fullPieceList.Front();
        }
        fullPiece = tempPieceList.Front();
        while (!IsNull(fullPiece)) {
            tempPieceList.PopFront();
            m_fullPieceList.PushBack(fullPiece);
            fullPiece = tempPieceList.Front();
        }

        tempPieceList.Clear();
        auto availPiece = m_availPieceList.Front();
        while (!IsNull(availPiece)) {
            m_availPieceList.PopFront();
            ShrinkPiece(availPiece);
            auto& piece = availPiece->m_value;
            if (piece.IsEmpty()) {
                emptyPieceList.PushBack(availPiece);
            }
            else {
                tempPieceList.PushBack(availPiece);
            }
            availPiece = m_availPieceList.Front();
        }
        availPiece = tempPieceList.Front();
        while (!IsNull(availPiece)) {
            tempPieceList.PopFront();
            m_availPieceList.PushBack(availPiece);
            availPiece = tempPieceList.Front();
        }

        auto emptyPiece = emptyPieceList.Front();
        while (!IsNull(emptyPiece)) {
            emptyPieceList.PopFront();
            if (pieceCount > 3) {
                DestoryPieceNode(emptyPiece);
                --pieceCount;
            }
            else {
                m_availPieceList.PushBack(emptyPiece);
            }
            emptyPiece = emptyPieceList.Front();
        }
    }

    void ShrinkPiece(PieceNode_t* pieceNode) {
        MZ_ASSERT_TRUE(!IsNull(pieceNode));

        auto& piece = pieceNode->m_value;
        UnitList_t restUnitList;

        auto unitNode = m_freeUnitList.Front();
        while (!IsNull(unitNode)) {
            m_freeUnitList.PopFront();
            if (piece.IsResponsible(unitNode)) {
                UnitNode_t* tUnitNode = unitNode;
                piece.Return(tUnitNode);
                if (piece.IsEmpty()) {
                    piece.Reset();
                    break;
                }
            }
            else {
                restUnitList.PushBack(unitNode);
            }
            unitNode = m_freeUnitList.Front();
        }

        auto restUnitNode = restUnitList.Front();
        while (!IsNull(restUnitNode)) {
            restUnitList.PopFront();
            m_freeUnitList.PushBack(restUnitNode);
            restUnitNode = restUnitList.Front();
        }
    }

private:
    PieceList_t m_fullPieceList;
    PieceList_t m_availPieceList;
    UnitList_t m_freeUnitList;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_SHRINKSTEPALLOCATOR_HPP_
