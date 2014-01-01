#ifndef MZ_SHARE_LISTITERATOR_HPP
#define MZ_SHARE_LISTITERATOR_HPP

#include "Metazion/Share/ShareInclude.hpp"

template<typename ValueType
, typename NodeType
>
class ListIterator {
    typedef ValueType Value_t;
    typedef NodeType Node_t;
    
public:
    ListIterator()
        : m_node(nullptr) {}

    ListIterator(const ListIterator& another)
        : m_node(another.m_node) {}

    ListIterator(Node_t* node)
        : m_node(node) {}

    ~ListIterator() {}

    ListIterator& operator =(const ListIterator& another) {
        if (&another != this) {
            m_node = another.m_node;
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
        Node_t* node = m_node->m_nextNode;
        return ListIterator(node);
    }

    bool operator ==(const ListIterator& another) const {
        return m_node == another.m_node;
    }

    bool operator !=(const ListIterator& another) const {
        return m_node != another.m_node;
    }

private:
    Node_t* m_node;
};

template<typename ValueType
, typename NodeType
>
class ListConstIterator {
    typedef ValueType Value_t;
    typedef NodeType Node_t;

public:
    ListConstIterator()
        : m_node(nullptr) {}

    ListConstIterator(const ListConstIterator& another)
        : m_node(another.m_node) {}

    ListConstIterator(const Node_t* node)
        : m_node(node) {}

    ~ListConstIterator() {}

    ListConstIterator& operator =(const ListConstIterator& another) {
        if (&another != this) {
            m_node = another.m_node;
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
        const Node_t* node = m_node->m_nextNode;
        return ListConstIterator(node);
    }

    bool operator ==(const ListConstIterator& another) const {
        return m_node == another.m_node;
    }

    bool operator !=(const ListConstIterator& another) const {
        return m_node != another.m_node;
    }

private:
    const Node_t* m_node;
};

#endif // MZ_SHARE_LISTITERATOR_HPP
