#ifndef MZ_SHARE_SELFLIST_HPP
#define MZ_SHARE_SELFLIST_HPP

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType>
struct SelfListNode {
    SelfListNode()
        : m_prevNode(nullptr)
        , m_nextNode(nullptr) {}

    ValueType m_value;
    SelfListNode* m_prevNode;
    SelfListNode* m_nextNode;
};

template<typename NodeType>
class SelfList {
    DISALLOW_COPY_AND_ASSIGN(SelfList)

    typedef NodeType Node_t;

public:
    SelfList()
        : m_headNode(nullptr)
        , m_tailNode(nullptr)
        , m_size(0) {}

    ~SelfList() {}

public:
    void Clear() {
        while (!IsEmpty()) {
            PopFront();
        }
    }

    bool IsEmpty() const {
        return nullptr == m_headNode;
    }

    int GetSize() const {
        return m_size;
    }

    const Node_t* Front() const {
        return m_headNode;
    }

    Node_t* Front() {
        return m_headNode;
    }

    const Node_t* Back() const {
        return m_tailNode;
    }

    Node_t* Back() {
        return m_tailNode;
    }

    void PopFront() {
        if (IsNull(m_headNode)) {
            ASSERT_TRUE(false);
            return;
        }

        Node_t* nextNode = m_headNode->m_nextNode;
        if (IsNull(nextNode)) {
            m_tailNode = nullptr;
        }
        else {
            nextNode->m_prevNode = nullptr;
        }
        Node_t* node = m_headNode;
        node->m_prevNode = nullptr;
        node->m_nextNode = nullptr;
        m_headNode = nextNode;
        --m_size;
    }

    void PopBack() {
        if (IsNull(m_tailNode)) {
            ASSERT_TRUE(false);
            return;
        }

        Node_t* prevNode = m_tailNode->m_prevNode;
        if (IsNull(prevNode)) {
            m_headNode = nullptr;
        }
        else {
            prevNode->m_nextNode = nullptr;
        }
        Node_t* node = m_tailNode;
        node->m_prevNode = nullptr;
        node->m_nextNode = nullptr;
        m_tailNode = prevNode;
        --m_size;
    }

    void PushFront(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

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
    }

    void PushBack(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

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
    }

    void InsertBefore(Node_t* location, Node_t* node) {
        ASSERT_TRUE(!IsNull(location));
        ASSERT_TRUE(!IsNull(node));
        ASSERT_TRUE(IsContain(location));
        ASSERT_TRUE(!IsContain(node));

        Node_t* prevNode = location->m_prevNode;
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
    }

    void InsertAfter(Node_t* location, Node_t* node) {
        ASSERT_TRUE(!IsNull(location));
        ASSERT_TRUE(!IsNull(node));
        ASSERT_TRUE(IsContain(location));
        ASSERT_TRUE(!IsContain(node));

        Node_t* nextNode = location->m_nextNode;
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
    }

    Node_t* Remove(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));
        ASSERT_TRUE(IsContain(node));

        Node_t* prevNode = node->m_prevNode;
        Node_t* nextNode = node->m_nextNode;
        if (IsNull(prevNode)) {
            ASSERT_TRUE(node == m_headNode);
            m_headNode = nextNode;
        }
        else {
            prevNode->m_nextNode = nextNode;
        }
        if (IsNull(nextNode)) {
            ASSERT_TRUE(node == m_tailNode);
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

    const Node_t* Forward(const Node_t* node) const {
        ASSERT_TRUE(!IsNull(node));

        return node->m_nextNode;
    }

    Node_t* Forward(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

        return node->m_nextNode;
    }

    const Node_t* Backward(const Node_t* node) const {
        ASSERT_TRUE(!IsNull(node));

        return node->m_prevNode;
    }

    Node_t* Backward(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

        return node->m_prevNode;
    }

    bool IsContain(Node_t* node) const {
        ASSERT_TRUE(!IsNull(node));

        const Node_t* cursor = Front();
        while (!IsNull(cursor)) {
            if (node == cursor) {
                return true;
            }
            cursor = Forward(cursor);
        }
        return false;
    }

private:
    Node_t* m_headNode;
    Node_t* m_tailNode;
    int m_size;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_SELFLIST_HPP
