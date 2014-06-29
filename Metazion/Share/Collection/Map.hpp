#ifndef _MZ_SHARE_MAP_HPP_
#define _MZ_SHARE_MAP_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/MapIterator.hpp"
#include "Metazion/Share/Collection/RBTree.hpp"
#include "Metazion/Share/Memory/HeapAllocator.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename KeyType
, typename ValueType
, typename CompareType = LessCompare<KeyType>
, typename AllocatorFamily = HeapAllocator<>
>
class Map {
    DISALLOW_COPY_AND_ASSIGN(Map)
    
    using Key_t = KeyType;
    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Data_t = Pair<Key_t, Value_t>;
    using DataCompare_t = PairCompare<Data_t, Compare_t>;
    using Node_t = RBTreeNode<Data_t>;
    using Tree_t = RBTree<Node_t, DataCompare_t>;
    using Allocator_t = typename AllocatorFamily::template Rebind<sizeof(Node_t)>;

public:
    using Iterator_t = MapIterator<Node_t>;
    using ConstIterator_t = MapConstIterator<Node_t>;

public:
    Map() { m_allocator.Initialize(); }

    ~Map() { m_allocator.Finalize(); }

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
    ConstIterator_t Begin() const {
        const auto node = m_tree.First();
        return ConstIterator_t(node);
    }

    Iterator_t Begin() {
        auto node = m_tree.First();
        return Iterator_t(node);
    }

    ConstIterator_t End() const {
        return ConstIterator_t();
    }

    Iterator_t End() {
        return Iterator_t();
    }

    Iterator_t Insert(const Key_t& key, const Value_t& value) {
        auto node = CreateNode();
        ASSERT_TRUE(!IsNull(node));
        node->m_value.first = key;
        node->m_value.second = value;
        const auto ret = m_tree.InsertUnique(node);
        if (IsNull(ret)) {
            DestoryNode(node);
        }

        return Iterator_t(ret);
    }

    void Erase(Iterator_t iter) {
        ASSERT_TRUE(iter != End());

        m_tree.Remove(iter.Node());
        DestoryNode(iter.Node());
    }

    void Erase(const Key_t& key) {
        auto iter = Find(key);
        if (iter != End()) {
            Erase(iter);
        }
    }

    Iterator_t Find(const Key_t& key) {
        Data_t data;
        data.first = key;
        Node_t* node = m_tree.Search(data);
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
