#ifndef _MZ_SHARE_SELFARRAYLIST_HPP_
#define _MZ_SHARE_SELFARRAYLIST_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType>
struct SelfArrayListNode {
    using Value_t = ValueType;

    Value_t m_value;
    int m_selfIndex = -1;
    int m_prevIndex = -1;
    int m_nextIndex = -1;
};

template<>
struct SelfArrayListNode<void> {
    int m_selfIndex = -1;
    int m_prevIndex = -1;
    int m_nextIndex = -1;
};

template<typename NodeType
, int INITSIZE
>
class SelfArrayList {
    using Node_t = NodeType;

    struct Entry {
        Node_t* m_node = nullptr;
        int m_freeNext = -1;
    };

public:
    SelfArrayList() {}

    ~SelfArrayList() {}

    SelfArrayList(const SelfArrayList& other) {
        *this = other;
    }

    SelfArrayList& operator =(const SelfArrayList& other) {
        if (&other != this) {
            for (auto index = 0; index < INITSIZE; ++index) {
                m_entries[index] = other.m_entries[index];
            }

            m_allocIndex = other.m_allocIndex;
            m_freeHead = other.m_freeHead;
            m_usedHead = other.m_usedHead;
            m_usedTail = other.m_usedTail;
            m_size = other.m_size;
        }
        return *this;
    }

    SelfArrayList(SelfArrayList&& other) {
        *this = std::move(other);
    }

    SelfArrayList& operator =(SelfArrayList&& other) {
        if (&other != this) {
            for (auto index = 0; index < INITSIZE; ++index) {
                m_entries[index] = other.m_entries[index];
            }

            m_allocIndex = other.m_allocIndex;
            m_freeHead = other.m_freeHead;
            m_usedHead = other.m_usedHead;
            m_usedTail = other.m_usedTail;
            m_size = other.m_size;
        }
        return *this;
    }

public:
    void Clear() {
        for (auto index = 0; index < INITSIZE; ++index) {
            m_entries[index] = {};
        }

        m_allocIndex = 0;
        m_freeHead = 0;
        m_usedHead = -1;
        m_usedTail = -1;
        m_size = 0;
    }

    bool IsEmpty() const {
        return -1 == m_usedHead;
    }

    int GetCapacity() const {
        return INITSIZE;
    }

    int GetSize() const {
        return m_size;
    }

    Node_t* Front() {
        if (IsInvalid(m_usedHead)) {
            return nullptr;
        }

        auto& entry = m_entries[m_usedHead];
        return entry.m_node;
    }

    const Node_t* Front() const {
        if (IsInvalid(m_usedHead)) {
            return nullptr;
        }

        auto& entry = m_entries[m_usedHead];
        return entry.m_node;
    }

    Node_t* Back() {
        if (IsInvalid(m_usedTail)) {
            return nullptr;
        }

        auto& entry = m_entries[m_usedHead];
        return entry.m_node;
    }

    const Node_t* Back() const {
        if (IsInvalid(m_usedTail)) {
            return nullptr;
        }

        auto& entry = m_entries[m_usedHead];
        return entry.m_node;
    }

    Node_t* PushFront(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(!IsContain(node));

        const auto index = NextFreeIndex();
        if (IsInvalid(index)) {
            return nullptr;
        }

        node->m_selfIndex = index;
        node->m_prevIndex = -1;
        node->m_nextIndex = -1;

        if (IsInvalid(m_usedHead)) {
            m_usedHead = node->m_selfIndex;
            m_usedTail = node->m_selfIndex;
        }
        else {
            Node(m_usedHead)->m_prevIndex = node->m_selfIndex;
            node->m_nextIndex = m_usedHead;
            m_usedHead = node->m_selfIndex;
        }

        m_entries[index].m_node = node;
        ++m_size;
        return node;
    }

    Node_t* PushBack(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(!IsContain(node));

        const auto index = NextFreeIndex();
        if (IsInvalid(index)) {
            return nullptr;
        }

        node->m_selfIndex = index;
        node->m_prevIndex = -1;
        node->m_nextIndex = -1;

        if (IsInvalid(m_usedTail)) {
            m_usedHead = node->m_selfIndex;
            m_usedTail = node->m_selfIndex;
        }
        else {
            node->m_prevIndex = m_usedTail;
            Node(m_usedTail)->m_nextIndex = node->m_selfIndex;
            m_usedTail = node->m_selfIndex;
        }

        m_entries[index].m_node = node;
        ++m_size;
        return node;
    }

    Node_t* PopFront() {
        if (IsInvalid(m_usedHead)) {
            return nullptr;
        }

        auto node = Node(m_usedHead);
        const auto index = node->m_selfIndex;

        m_usedHead = node->m_nextIndex;
        if (IsInvalid(m_usedHead)) {
            m_usedTail = -1;
        }
        else {
            Node(m_usedHead)->m_prevIndex = -1;
        }

        node->m_selfIndex = -1;
        node->m_prevIndex = -1;
        node->m_nextIndex = -1;

        BackFreeHead(index);

        m_entries[index].m_node = nullptr;
        --m_size;
        return node;
    }

    Node_t* PopBack() {
        if (IsInvalid(m_usedTail)) {
            return nullptr;
        }

        auto node = Node(m_usedTail);
        const auto index = node->m_selfIndex;

        m_usedTail = node->m_prevIndex;
        if (IsInvalid(m_usedTail)) {
            m_usedHead = -1;
        }
        else {
            Node(m_usedTail)->m_nextIndex = -1;
        }
        node->m_selfIndex = -1;
        node->m_prevIndex = -1;
        node->m_nextIndex = -1;

        BackFreeHead(index);

        m_entries[index].m_node = nullptr;
        --m_size;
        return node;
    }

    Node_t* InsertBefore(Node_t* location, Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(location));
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(IsContain(location));
        MZ_ASSERT_TRUE(!IsContain(node));

        const auto index = NextFreeIndex();
        if (IsInvalid(index)) {
            return nullptr;
        }

        node->m_selfIndex = index;
        node->m_prevIndex = -1;
        node->m_nextIndex = -1;

        auto prevIndex = location->m_prevIndex;
        if (IsInvalid(prevIndex)) {
            m_usedHead = node->m_selfIndex;
        }
        else {
            Node(prevIndex)->m_nextIndex = node->m_selfIndex;
        }
        location->m_prevIndex = node->m_selfIndex;
        node->m_prevIndex = prevIndex;
        node->m_nextIndex = location->m_selfIndex;

        m_entries[index].m_node = node;
        ++m_size;
        return node;
    }

    Node_t* InsertAfter(Node_t* location, Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(location));
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(IsContain(location));
        MZ_ASSERT_TRUE(!IsContain(node));

        const auto index = NextFreeIndex();
        if (IsInvalid(index)) {
            return nullptr;
        }

        node->m_selfIndex = index;
        node->m_prevIndex = -1;
        node->m_nextIndex = -1;

        auto nextIndex = location->m_nextIndex;
        if (IsInvalid(nextIndex)) {
            m_usedTail = node->m_selfIndex;
        }
        else {
            Node(nextIndex)->m_prevIndex = node->m_selfIndex;
        }
        location->m_nextIndex = node->m_selfIndex;
        node->m_prevIndex = location->m_selfIndex;
        node->m_nextIndex = nextIndex;

        m_entries[index].m_node = node;
        ++m_size;
        return node;
    }

    Node_t* Remove(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(IsContain(node));

        const auto index = node->m_selfIndex;
        const auto prevIndex = node->m_prevIndex;
        const auto nextIndex = node->m_nextIndex;
        if (IsInvalid(prevIndex)) {
            MZ_ASSERT_TRUE(node->m_selfIndex == m_usedHead);
            m_usedHead = nextIndex;
        }
        else {
            Node(prevIndex)->m_nextIndex = nextIndex;
        }
        if (IsInvalid(nextIndex)) {
            MZ_ASSERT_TRUE(node->m_selfIndex == m_usedTail);
            m_usedTail = prevIndex;
        }
        else {
            Node(nextIndex)->m_prevIndex = prevIndex;
        }

        node->m_selfIndex = -1;
        node->m_prevIndex = -1;
        node->m_nextIndex = -1;

        BackFreeHead(index);

        m_entries[index].m_node = nullptr;
        --m_size;
        return Node(nextIndex);
    }

    Node_t* Backward(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));

        return Node(node->m_prevIndex);
    }

    const Node_t* Backward(const Node_t* node) const {
        MZ_ASSERT_TRUE(!IsNull(node));

        return Node(node->m_prevIndex);
    }

    Node_t* Forward(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));

        return Node(node->m_nextIndex);
    }

    const Node_t* Forward(const Node_t* node) const {
        MZ_ASSERT_TRUE(!IsNull(node));

        return Node(node->m_nextIndex);
    }

    bool IsContain(Node_t* node) const {
        MZ_ASSERT_TRUE(!IsNull(node));

        auto cursor = Front();
        while (!IsNull(cursor)) {
            if (node == cursor) {
                return true;
            }
            cursor = Forward(cursor);
        }

        return false;
    }

private:
    int NextFreeIndex() {
        if (m_freeHead < 0) {
            return -1;
        }

        const auto index = m_freeHead;
        if (m_freeHead < m_allocIndex) {
            auto& entry = m_entries[m_freeHead];
            m_freeHead = entry.m_freeNext;
        }
        else {
            m_freeHead += 1;
            m_allocIndex = m_freeHead;
        }

        if (m_freeHead >= INITSIZE) {
            m_freeHead = -1;
        }

        return index;
    }

    int BackFreeHead(int index) {
        m_entries[index].m_freeNext = m_freeHead;
        m_freeHead = index;
        return m_freeHead;
    }

    Node_t* Node(int index) {
        if (IsInvalid(index)) {
            return nullptr;
        }

        return m_entries[index].m_node;
    }

    const Node_t* Node(int index) const {
        if (IsInvalid(index)) {
            return nullptr;
        }

        return m_entries[index].m_node;
    }

    bool IsInvalid(int index) const {
        return index < 0 || index >= INITSIZE;
    }

private:
    Entry m_entries[INITSIZE];
    int m_allocIndex = 0;
    int m_freeHead = 0;
    int m_usedHead = -1;
    int m_usedTail = -1;
    int m_size = 0;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_SELFARRAYLIST_HPP_
