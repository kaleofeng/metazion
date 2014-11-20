#ifndef _MZ_SHARE_MAP_HPP_
#define _MZ_SHARE_MAP_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/RBTree.hpp"
#include "Metazion/Share/Memory/HeapAllocator.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename KeyType
, typename ValueType
, typename CompareType = LessCompare<KeyType>
, typename AllocatorFamily = HeapAllocator<>
>
class Map {
    using Key_t = KeyType;
    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Entry_t = Pair<Key_t, Value_t>;
    using EntryCompare_t = PairCompare<Entry_t, Compare_t>;
    using Node_t = RBTreeNode<Entry_t>;
    using Tree_t = RBTree<Node_t, EntryCompare_t>;
    using Allocator_t = typename AllocatorFamily::template Rebind<sizeof(Node_t)>;

    class Iterator {
        friend class Map;

    public:
        Iterator()
            : m_node(nullptr) {}

        Iterator(const Iterator& other)
            : m_node(other.m_node) {}

        Iterator(Node_t* node)
            : m_node(node) {}

        ~Iterator() {}

        Iterator& operator =(const Iterator& other) {
            if (&other != this) {
                m_node = other.m_node;
            }
            return *this;
        }

        auto& operator *() {
            return m_node->m_value;
        }

        auto* operator ->() {
            return &m_node->m_value;
        }

        Iterator& operator ++() {
            m_node = m_node->Forward();
            return *this;
        }

        Iterator operator ++(int) {
            auto temp = *this;
            m_node = m_node->Forward();
            return temp;
        }

        bool operator ==(const Iterator& other) const {
            return m_node == other.m_node;
        }

        bool operator !=(const Iterator& other) const {
            return !operator ==(other);
        }

    private:
        Node_t* m_node;
    };

public:
    using Iterator_t = Iterator;

public:
    Map() {}

    ~Map() {}

    Map(Map& other)
        : Map() {
        *this = other;
    }

    Map& operator =(Map& other) {
        if (&other != this) {
            Clear();
            for (auto& value : other) {
                Insert(value.first, value.second);
            }
        }
        return *this;
    }

    Map(Map&& other)
        : Map() {
        *this = std::move(other);
    }

    Map& operator =(Map&& other) {
        *this = other;
        return *this;
    }

public:
    void Clear() {
        while (!IsEmpty()) {
            Iterator_t iter = Begin();
            Erase(iter);
        }
    }

    bool IsEmpty() const {
        return m_tree.IsEmpty();
    }

    int GetSize() const {
        return m_tree.GetSize();
    }

public:
    ADAPT_FOR_RANGE_TRAVERSAL()

    Iterator_t Begin() {
        auto node = m_tree.First();
        return Iterator_t(node);
    }

    Iterator_t End() {
        return Iterator_t();
    }

    Iterator_t Insert(const Key_t& key, const Value_t& value) {
        auto node = CreateNode();
        ASSERT_TRUE(!IsNull(node));
        node->m_value.first = key;
        node->m_value.second = value;
        auto curNode = m_tree.InsertUnique(node);
        if (IsNull(curNode)) {
            DestoryNode(node);
        }

        return Iterator_t(curNode);
    }

    Iterator_t Erase(Iterator_t iter) {
        ASSERT_TRUE(iter != End());

        auto nextNode = m_tree.Remove(iter.m_node);
        DestoryNode(iter.m_node);
        return Iterator_t(nextNode);
    }

    Iterator_t Find(const Key_t& key) {
        Entry_t entry;
        entry.first = key;
        auto node = m_tree.Search(entry);
        return Iterator_t(node);
    }

private:
    Node_t* CreateNode() {
        auto memory = m_allocator.Alloc();
        auto node = new(memory) Node_t();
        return node;
    }

    void DestoryNode(Node_t* node) {
        node->~Node_t();
        m_allocator.Free(node);
    }

private:
    Allocator_t m_allocator;
    Tree_t m_tree;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_MAP_HPP_
