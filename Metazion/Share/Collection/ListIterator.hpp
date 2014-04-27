#ifndef _MZ_SHARE_LISTITERATOR_HPP_
#define _MZ_SHARE_LISTITERATOR_HPP_

#include "Metazion/Share/ShareInclude.hpp"

template<typename ValueType
, typename NodeType
>
class ListIterator {
    using Value_t = ValueType;
    using Node_t = NodeType;
    
public:
    ListIterator()
        : m_node(nullptr) {}

    ListIterator(const ListIterator& other)
        : m_node(other.m_node) {}

    ListIterator(Node_t* node)
        : m_node(node) {}

    ~ListIterator() {}

    ListIterator& operator =(const ListIterator& other) {
        if (&other != this) {
            m_node = other.m_node;
        }
        return *this;
    }

    Node_t* Node() {
        return m_node;
    }

    Value_t& operator *() {
        return m_node->m_value;
    }

    Value_t* operator ->() {
        return &m_node->m_value;
    }

    ListIterator& operator ++() {
        m_node = m_node->m_nextNode;
        return *this;
    }

    ListIterator& operator ++(int) {
        auto node = m_node->m_nextNode;
        return ListIterator(node);
    }

    bool operator ==(const ListIterator& other) const {
        return m_node == other.m_node;
    }

    bool operator !=(const ListIterator& other) const {
        return m_node != other.m_node;
    }

private:
    Node_t* m_node;
};

template<typename ValueType
, typename NodeType
>
class ListConstIterator {
    using Value_t = ValueType;
    using Node_t = NodeType;

public:
    ListConstIterator()
        : m_node(nullptr) {}

    ListConstIterator(const ListConstIterator& other)
        : m_node(other.m_node) {}

    ListConstIterator(const Node_t* node)
        : m_node(node) {}

    ~ListConstIterator() {}

    ListConstIterator& operator =(const ListConstIterator& other) {
        if (&other != this) {
            m_node = other.m_node;
        }
        return *this;
    }

    const Node_t* Node() const {
        return m_node;
    }

    const Value_t& operator *() const {
        return m_node->m_value;
    }

    const Value_t* operator ->() const {
        return &m_node->m_value;
    }

    ListConstIterator& operator ++() {
        m_node = m_node->m_nextNode;
        return *this;
    }

    ListConstIterator& operator ++(int) {
        const auto node = m_node->m_nextNode;
        return ListConstIterator(node);
    }

    bool operator ==(const ListConstIterator& other) const {
        return m_node == other.m_node;
    }

    bool operator !=(const ListConstIterator& other) const {
        return m_node != other.m_node;
    }

private:
    const Node_t* m_node;
};

#endif // _MZ_SHARE_LISTITERATOR_HPP_
