#ifndef _MZ_SHARE_RBTREE_HPP_
#define _MZ_SHARE_RBTREE_HPP_

#include "Metazion/Share/ShareInclude.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType>
struct RBTreeNode {
    using Value_t = ValueType;

    enum Color{
        RED = 0,
        BLACK,
    };

    RBTreeNode* m_parent = nullptr;
    RBTreeNode* m_right = nullptr;
    RBTreeNode* m_left = nullptr;
    Color m_color = RED;
    
    Value_t m_value;

    bool IsRed() const {
        return m_color == RED;
    }

    void SetRed() {
        m_color = RED;
    }

    bool IsBlack() const {
        return m_color == BLACK;
    }

    void SetBlack() {
        m_color = BLACK;
    }

    RBTreeNode* Backward() {
        auto node = this;
        if (node->m_parent == node) {
            return nullptr;
        }

        if (!IsNull(node->m_left)) {
            node = node->m_left;
            while (!IsNull(node->m_right)) {
                node = node->m_right;
            }

            return node;
        }

        auto parent = node->m_parent;
        while (!IsNull(parent) && node == parent->m_left) {
            node = parent;
            parent = node->m_parent;
        }

        return parent;
    }

    RBTreeNode* Forward() {
        auto node = this;
        if (node->m_parent == node) {
            return nullptr;
        }

        if (!IsNull(node->m_right)) {
            node = node->m_right;
            while (!IsNull(node->m_left)) {
                node = node->m_left;
            }

            return node;
        }

        auto parent = node->m_parent;
        while (!IsNull(parent) && node == parent->m_right) {
            node = parent;
            parent = node->m_parent;
        }

        return parent;
    }
};

template<typename NodeType
, typename CompareType = LessCompare<typename NodeType::Value_t>
>
class RBTree {
    DISALLOW_COPY_AND_ASSIGN(RBTree)

    using Node_t = NodeType;
    using Compare_t = CompareType;
    using Value_t = typename Node_t::Value_t;

public:
    RBTree()
        : m_root(nullptr)
        , m_size(0) {}

    ~RBTree() {}

public:
    void Clear() {
        m_root = nullptr;
        m_size = 0;
    }

    bool IsEmpty() const {
        return nullptr == m_root;
    }

    int GetSize() const {
        return m_size;
    }

    Node_t* First() {
        auto next = m_root;
        if (IsNull(next)) {
            return nullptr;
        }

        while (!IsNull(next->m_left)) {
            next = next->m_left;
        }

        return next;
    }

    Node_t* Last() {
        auto next = m_root;
        if (IsNull(next)) {
            return nullptr;
        }

        while (!IsNull(next->m_right)) {
            next = next->m_right;
        }

        return next;
    }

    Node_t* InsertUnique(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

        Node_t* parent = nullptr;
        auto temp = &m_root;

        while (!IsNull(*temp)) {
            parent = *temp;

            const auto ret = m_compare(node->m_value, parent->m_value);
            if (ret < 0) {
                temp = &(*temp)->m_left;
            }
            else if (ret > 0) {
                temp = &(*temp)->m_right;
            }
            else  {
                return nullptr;
            }
        }

        Link(node, parent, *temp);
        InsertColor(node);
        ++m_size;
        return node;
    }

    Node_t* InsertRepeat(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

        Node_t* parent = nullptr;
        auto temp = &m_root;

        while (!IsNull(*temp)) {
            parent = *temp;

            const auto ret = m_compare(node->m_value, parent->m_value);
            if (ret < 0) {
                temp = &(*temp)->m_left;
            }
            else {
                temp = &(*temp)->m_right;
            }
        }

        Link(node, parent, *temp);
        InsertColor(node);
        ++m_size;
        return node;
    }

    Node_t* Remove(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

        auto forward = node->Forward();

        Node_t* child = nullptr;
        Node_t* parent = nullptr;
        auto color = Node_t::RED;

        do {
            if (IsNull(node->m_left)) {
                child = node->m_right;
            }
            else if (IsNull(node->m_right)) {
                child = node->m_left;
            }
            else {
                auto old = node;
                node = node->m_right;

                auto left = node->m_left;
                while (!IsNull(left)) {
                    node = left;
                    left = node->m_left;
                }

                auto oldParent = old->m_parent;
                if (!IsNull(oldParent)) {
                    if (oldParent->m_left == old) {
                        oldParent->m_left = node;
                    }
                    else {
                        oldParent->m_right = node;
                    }
                }
                else {
                    m_root = node;
                }

                child = node->m_right;
                parent = node->m_parent;
                color = node->m_color;

                if (parent == old) {
                    parent = node;
                }
                else {
                    if (!IsNull(child)) {
                        child->m_parent = parent;
                    }

                    parent->m_left = child;
                    node->m_right = old->m_right;
                    old->m_right->m_parent = node;
                }

                node->m_color = old->m_color;
                node->m_parent = old->m_parent;
                node->m_left = old->m_left;
                old->m_left->m_parent = node;

                break;
            }

            parent = node->m_parent;
            color = node->m_color;

            if (!IsNull(child)) {
                child->m_parent = parent;
            }

            if (!IsNull(parent)) {
                if (parent->m_left == node) {
                    parent->m_left = child;
                }
                else {
                    parent->m_right = child;
                }
            }
            else {
                m_root = child;
            }
        } while (false);
        
        if (color == Node_t::BLACK) {
            RemoveColor(child, parent);
        }

        --m_size;
        return forward;
    }

    Node_t* Search(const Value_t& value) {
        auto node = m_root;
        while (!IsNull(node)) {
            const auto ret = m_compare(value, node->m_value);
            if (ret < 0) {
                node = node->m_left;
            }
            else if (ret > 0) {
                node = node->m_right;
            }
            else {
                return node;
            }
        }

        return nullptr;
    }

    Node_t* Replace(Node_t* victim, Node_t* replacement) {
        ASSERT_TRUE(!IsNull(victim));
        ASSERT_TRUE(!IsNull(replacement));

        auto parent = victim->m_parent;
        if (!IsNull(parent)) {
            if (victim == parent->m_left) {
                parent->m_left = replacement;
            }
            else {
                parent->m_right = replacement;
            }
        }
        else {
            m_root = replacement;
        }

        if (!IsNull(victim->m_left)) {
            victim->m_left->m_parent = replacement;
        }

        if (!IsNull(victim->m_right)) {
            victim->m_right->m_parent = replacement;
        }

        *replacement = *victim;
        return victim;
    }

    Node_t* Backward(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

        return node->Backward();
    }

    Node_t* Forward(Node_t* node) {
        ASSERT_TRUE(!IsNull(node));

        return node->Forward();
    }

private:

    void Link(Node_t* node, Node_t* parent, Node_t*& link) {
        node->m_parent = parent;
        node->m_left = node->m_right = nullptr;

        link = node;
    }

    void InsertColor(Node_t* node) {
        auto parent = node->m_parent;
        while (!IsNull(parent) && parent->IsRed()) {
            auto gparent = parent->m_parent;
            if (parent == gparent->m_left) {
                auto uncle = gparent->m_right;
                if (uncle && uncle->IsRed()) {
                    uncle->SetBlack();
                    parent->SetBlack();
                    gparent->SetRed();
                    node = gparent;
                    continue;
                }

                if (parent->m_right == node) {
                    RotateLeft(parent);
                    auto temp = parent;
                    parent = node;
                    node = temp;
                }

                parent->SetBlack();
                gparent->SetRed();
                RotateRight(gparent);
            }
            else {
                auto uncle = gparent->m_left;
                if (uncle && uncle->IsRed()) {
                    uncle->SetBlack();
                    parent->SetBlack();
                    gparent->SetRed();
                    node = gparent;
                    continue;
                }

                if (parent->m_left == node) {
                    RotateRight(parent);
                    auto temp = parent;
                    parent = node;
                    node = temp;
                }

                parent->SetBlack();
                gparent->SetRed();
                RotateLeft(gparent);
            }

            parent = node->m_parent;
        }

        m_root->SetBlack();
    }

    void RemoveColor(Node_t* node, Node_t* parent) {
        while ((IsNull(node) || node->IsBlack()) && node != m_root) {
            if (parent->m_left == node) {
                auto other = parent->m_right;
                if (other->IsRed()) {
                    other->SetBlack();
                    parent->SetRed();
                    RotateLeft(parent);
                    other = parent->m_right;
                }

                if ((IsNull(other->m_left) || other->m_left->IsBlack()) &&
                    (IsNull(other->m_right) || other->m_right->IsBlack())) {
                    other->SetRed();
                    node = parent;
                    parent = node->m_parent;
                }
                else {
                    if (IsNull(other->m_right) || other->m_right->IsBlack()) {
                        other->m_left->SetBlack();
                        other->SetRed();
                        RotateRight(other);
                        other = parent->m_right;
                    }

                    other->m_color = parent->m_color;
                    parent->SetBlack();
                    other->m_right->SetBlack();
                    RotateLeft(parent);
                    node = m_root;
                    break;
                }
            }
            else {
                auto other = parent->m_left;
                if (other->IsRed()) {
                    other->SetBlack();
                    parent->SetRed();
                    RotateRight(parent);
                    other = parent->m_left;
                }

                if ((IsNull(other->m_left) || other->m_left->IsBlack()) &&
                    (IsNull(other->m_right) || other->m_right->IsBlack())) {
                    other->SetRed();
                    node = parent;
                    parent = node->m_parent;
                }
                else {
                    if (IsNull(other->m_left) || other->m_left->IsBlack()) {
                        other->m_right->SetBlack();
                        other->SetRed();
                        RotateLeft(other);
                        other = parent->m_left;
                    }

                    other->m_color = parent->m_color;
                    parent->SetBlack();
                    other->m_left->SetBlack();
                    RotateRight(parent);
                    node = m_root;
                    break;
                }
            }
        }

        if (!IsNull(node)) {
            node->SetBlack();
        }    
    }

    void RotateLeft(Node_t* node) {
        auto right = node->m_right;
        auto parent = node->m_parent;
        node->m_right = right->m_left;

        if (!IsNull(right->m_left)) {
            right->m_left->m_parent = node;
        }

        right->m_left = node;
        right->m_parent = parent;

        if (!IsNull(parent)) {
            if (node == parent->m_left) {
                parent->m_left = right;
            }
            else {
                parent->m_right = right;
            }
        }
        else {
            m_root = right;
        }

        node->m_parent = right;
    }

    void RotateRight(Node_t* node) {
        auto left = node->m_left;
        auto parent = node->m_parent;
        node->m_left = left->m_right;

        if (!IsNull(left->m_right)) {
            left->m_right->m_parent = node;
        }

        left->m_right = node;
        left->m_parent = parent;

        if (!IsNull(parent)) {
            if (node == parent->m_right) {
                parent->m_right = left;
            }
            else {
                parent->m_left = left;
            }
        }
        else {
            m_root = left;
        }

        node->m_parent = left;
    }

private:
    Compare_t m_compare;
    Node_t* m_root;
    int m_size;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_RBTREE_HPP_
