#ifndef _MZ_SHARE_SELFLIST_HPP_
#define _MZ_SHARE_SELFLIST_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType>
struct SelfListNode {
    using Value_t = ValueType;

    Value_t m_value;
    SelfListNode* m_prevNode = nullptr;
    SelfListNode* m_nextNode = nullptr;
};

template<>
struct SelfListNode<void> {
    SelfListNode* m_prevNode = nullptr;
    SelfListNode* m_nextNode = nullptr;
};

template<typename NodeType>
class SelfList {
    using Node_t = NodeType;

public:
    SelfList() {}

    ~SelfList() {}

    SelfList(const SelfList& other) {
        *this = other;
    }

    SelfList& operator =(const SelfList& other) {
        if (&other != this) {
            m_headNode = other.m_headNode;
            m_tailNode = other.m_tailNode;
            m_size = other.m_size;
        }
        return *this;
    }
    
    SelfList(SelfList&& other) {
        *this = std::move(other);
    }

    SelfList& operator =(SelfList&& other) {
        if (&other != this) {
            m_headNode = other.m_headNode;
            m_tailNode = other.m_tailNode;
            m_size = other.m_size;

            other.m_headNode = nullptr;
            other.m_tailNode = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

public:
    void Clear() {
        m_headNode = nullptr;
        m_tailNode = nullptr;
        m_size = 0;
    }

    bool IsEmpty() const {
        return nullptr == m_headNode;
    }

    int GetSize() const {
        return m_size;
    }

    Node_t* Front() {
        return m_headNode;
    }

    const Node_t* Front() const {
        return m_headNode;
    }

    Node_t* Back() {
        return m_tailNode;
    }

    const Node_t* Back() const {
        return m_tailNode;
    }

    Node_t* PushFront(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(!IsContain(node));

        node->m_prevNode = nullptr;
        node->m_nextNode = nullptr;
        if (IsNull(m_headNode)) {
            m_headNode = node;
            m_tailNode = node;
        }
        else {
            m_headNode->m_prevNode = node;
            node->m_nextNode = m_headNode;
            m_headNode = node;
        }
        ++m_size;
        return node;
    }

    Node_t* PushBack(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(!IsContain(node));

        node->m_prevNode = nullptr;
        node->m_nextNode = nullptr;
        if (IsNull(m_tailNode)) {
            m_headNode = node;
            m_tailNode = node;
        }
        else {
            node->m_prevNode = m_tailNode;
            m_tailNode->m_nextNode = node;
            m_tailNode = node;
        }
        ++m_size;
        return node;
    }

    Node_t* PopFront() {
        if (IsNull(m_headNode)) {
            return nullptr;
        }

        auto node = m_headNode;
        m_headNode = m_headNode->m_nextNode;
        if (IsNull(m_headNode)) {
            m_tailNode = nullptr;
        }
        else {
            m_headNode->m_prevNode = nullptr;
        }
        node->m_prevNode = nullptr;
        node->m_nextNode = nullptr;
        --m_size;
        return node;
    }

    Node_t* PopBack() {
        if (IsNull(m_tailNode)) {
            return nullptr;
        }

        auto node = m_tailNode;
        m_tailNode = m_tailNode->m_prevNode;
        if (IsNull(m_tailNode)) {
            m_headNode = nullptr;
        }
        else {
            m_tailNode->m_nextNode = nullptr;
        }
        node->m_prevNode = nullptr;
        node->m_nextNode = nullptr;
        --m_size;
        return node;
    }

    Node_t* InsertBefore(Node_t* location, Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(location));
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(IsContain(location));
        MZ_ASSERT_TRUE(!IsContain(node));

        auto prevNode = location->m_prevNode;
        if (IsNull(prevNode)) {
            m_headNode = node;
        }
        else {
            prevNode->m_nextNode = node;
        }
        location->m_prevNode = node;
        node->m_prevNode = prevNode;
        node->m_nextNode = location;
        ++m_size;
        return node;
    }

    Node_t* InsertAfter(Node_t* location, Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(location));
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(IsContain(location));
        MZ_ASSERT_TRUE(!IsContain(node));

        auto nextNode = location->m_nextNode;
        if (IsNull(nextNode)) {
            m_tailNode = node;
        }
        else {
            nextNode->m_prevNode = node;
        }
        location->m_nextNode = node;
        node->m_prevNode = location;
        node->m_nextNode = nextNode;
        ++m_size;
        return node;
    }

    Node_t* Remove(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(IsContain(node));

        auto prevNode = node->m_prevNode;
        auto nextNode = node->m_nextNode;
        if (IsNull(prevNode)) {
            MZ_ASSERT_TRUE(node == m_headNode);
            m_headNode = nextNode;
        }
        else {
            prevNode->m_nextNode = nextNode;
        }
        if (IsNull(nextNode)) {
            MZ_ASSERT_TRUE(node == m_tailNode);
            m_tailNode = prevNode;
        }
        else {
            nextNode->m_prevNode = prevNode;
        }
        node->m_prevNode = nullptr;
        node->m_nextNode = nullptr;
        --m_size;
        return nextNode;
    }

    Node_t* Backward(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));

        return node->m_prevNode;
    }

    const Node_t* Backward(const Node_t* node) const {
        MZ_ASSERT_TRUE(!IsNull(node));

        return node->m_prevNode;
    }

    Node_t* Forward(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));

        return node->m_nextNode;
    }

    const Node_t* Forward(const Node_t* node) const {
        MZ_ASSERT_TRUE(!IsNull(node));

        return node->m_nextNode;
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
    Node_t* m_headNode = nullptr;
    Node_t* m_tailNode = nullptr;
    int m_size = 0;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_SELFLIST_HPP_
