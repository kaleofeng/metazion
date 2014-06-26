#ifndef _MZ_SHARE_MAPITERATOR_HPP_
#define _MZ_SHARE_MAPITERATOR_HPP_

#include "Metazion/Share/ShareInclude.hpp"

template<typename NodeType>
class MapIterator {
    using Node_t = NodeType;
    using Value_t = typename Node_t::Value_t;
    
public:
    MapIterator()
        : m_node(nullptr) {}

    MapIterator(const MapIterator& other)
        : m_node(other.m_node) {}

    MapIterator(Node_t* node)
        : m_node(node) {}

    ~MapIterator() {}

    MapIterator& operator =(const MapIterator& other) {
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

    MapIterator& operator ++() {
        m_node = m_node->Forward();
        return *this;
    }

    MapIterator& operator ++(int) {
        auto node = m_node->Forward();
        return MapIterator(node);
    }

    bool operator ==(const MapIterator& other) const {
        return m_node == other.m_node;
    }

    bool operator !=(const MapIterator& other) const {
        return m_node != other.m_node;
    }

private:
    Node_t* m_node;
};

template<typename NodeType>
class MapConstIterator {
    using Node_t = NodeType;
    using Value_t = typename Node_t::Value_t;

public:
    MapConstIterator()
        : m_node(nullptr) {}

    MapConstIterator(const MapConstIterator& other)
        : m_node(other.m_node) {}

    MapConstIterator(const Node_t* node)
        : m_node(node) {}

    ~MapConstIterator() {}

    MapConstIterator& operator =(const MapConstIterator& other) {
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

    MapConstIterator& operator ++() {
        m_node = m_node->Forward();
        return *this;
    }

    MapConstIterator& operator ++(int) {
        const auto node = m_node->Forward();
        return MapConstIterator(node);
    }

    bool operator ==(const MapConstIterator& other) const {
        return m_node == other.m_node;
    }

    bool operator !=(const MapConstIterator& other) const {
        return m_node != other.m_node;
    }

private:
    const Node_t* m_node;
};

#endif // _MZ_SHARE_MAPITERATOR_HPP_
