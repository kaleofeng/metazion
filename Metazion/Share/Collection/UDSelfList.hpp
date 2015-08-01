#ifndef _MZ_SHARE_UDSELFLIST_HPP_
#define _MZ_SHARE_UDSELFLIST_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType>
struct UDSelfListNode {
    using Value_t = ValueType;

    Value_t m_value;
    UDSelfListNode* m_nextNode = nullptr;
};

template<>
struct UDSelfListNode<void> {
    UDSelfListNode* m_nextNode = nullptr;
};

template<typename NodeType>
class UDSelfList {
    using Node_t = NodeType;

public:
    UDSelfList() {}

    ~UDSelfList() {}

    UDSelfList(const UDSelfList& other)
        : UDSelfList() {
        *this = other;
    }

    UDSelfList& operator =(const UDSelfList& other) {
        if (&other != this) {
            m_headNode = other.m_headNode;
            m_tailNode = other.m_tailNode;
            m_size = other.m_size;
        }
        return *this;
    }

    UDSelfList(UDSelfList&& other)
        : UDSelfList() {
        *this = std::move(other);
    }

    UDSelfList& operator =(UDSelfList&& other) {
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

    Node_t* PushFront(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));

        node->m_nextNode = nullptr;
        if (IsNull(m_headNode)) {
            m_headNode = node;
            m_tailNode = node;
        }
        else {
            node->m_nextNode = m_headNode;
            m_headNode = node;
        }
        ++m_size;
        return node;
    }

    Node_t* PushBack(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));

        node->m_nextNode = nullptr;
        if (IsNull(m_tailNode)) {
            m_headNode = node;
            m_tailNode = node;
        }
        else {
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
        node->m_nextNode = nullptr;
        --m_size;
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
        location->m_nextNode = node;
        node->m_nextNode = nextNode;
        ++m_size;
        return node;
    }

    Node_t* Remove(Node_t* node) {
        MZ_ASSERT_TRUE(!IsNull(node));
        MZ_ASSERT_TRUE(IsContain(node));

        Node_t* prevNode = nullptr;
        auto curNode = m_headNode;
        while (!IsNull(curNode)) {
            auto nextNode = curNode->m_nextNode;
            if (node != curNode) {
                prevNode = curNode;
                curNode = nextNode;
                continue;
            }

            if (IsNull(prevNode)) {
                m_headNode = nextNode;
                if (IsNull(nextNode)) {
                    m_tailNode = nullptr;
                }
            }
            else {
                prevNode->m_nextNode = nextNode;
                if (IsNull(nextNode)) {
                    m_tailNode = prevNode;
                }
            }
          
            node->m_nextNode = nullptr;
            --m_size;
            return nextNode;
        }

        MZ_ASSERT_TRUE(false);
        return nullptr;
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

#endif // _MZ_SHARE_UDSELFLIST_HPP_
