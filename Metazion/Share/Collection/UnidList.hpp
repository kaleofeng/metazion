#ifndef MZ_SHARE_UNIDLIST_HPP
#define MZ_SHARE_UNIDLIST_HPP

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Memory/HeapAllocator.hpp"
#include "Metazion/Share/Collection/ListIterator.hpp"
#include "Metazion/Share/Collection/UnidSelfList.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ValueType
, typename AllocatorFamily = HeapAllocator<>
>
class UnidList {
    DISALLOW_COPY_AND_ASSIGN(UnidList)

    typedef ValueType Value_t;
    typedef UnidSelfListNode<Value_t> Node_t;
    typedef typename AllocatorFamily::template
        Rebind<sizeof(Node_t)>::Allocator_t Allocator_t;
    typedef UnidSelfList<Node_t> List_t;

public:
    typedef ListIterator<Value_t, Node_t> Iterator_t;
    typedef ListConstIterator<Value_t, Node_t> ConstIterator_t;

public:
    UnidList() { m_allocator.Initialize(); }

    ~UnidList() { m_allocator.Finalize(); }

public:
    void Clear() {
        while (!IsEmpty()) {
            PopFront();
        }
    }

    bool IsEmpty() const {
        return m_list.IsEmpty();
    }

    int GetSize() const {
        return m_list.GetSize();
    }

    const Value_t& Front() const {
        Node_t* node = m_list.Front();
        return node->m_value;
    }

    Value_t& Front() {
        Node_t* node = m_list.Front();
        return node->m_value;
    }

    const Value_t& Back() const {
        Node_t* node = m_list.Back();
        return node->m_value;
    }

    Value_t& Back() {
        Node_t* node = m_list.Back();
        return node->m_value;
    }

    void PopFront() {
        Node_t* node = m_list.Front();
        if (IsNull(node)) {
            ASSERT_TRUE(false);
            return;
        }

        m_list.PopFront();
        DestoryNode(node);
    }

    void PushFront(const Value_t& value) {
        Node_t* node = CreateNode();
        ASSERT_TRUE(!IsNull(node));
        node->m_value = value;
        m_list.PushFront(node);
    }

    void PushBack(const Value_t& value) {
        Node_t* node = CreateNode();
        ASSERT_TRUE(!IsNull(node));
        node->m_value = value;
        m_list.PushBack(node);
    }

public:
    ConstIterator_t Begin() const {
        const Node_t* node = m_list.Front();
        return ConstIterator_t(node);
    }

    Iterator_t Begin() {
        Node_t* node = m_list.Front();
        return Iterator_t(node);
    }

    ConstIterator_t End() const {
        return ConstIterator_t();
    }

    Iterator_t End() {
        return Iterator_t();
    }

    Iterator_t InsertAfter(Iterator_t iter, const Value_t& value) {
        Node_t* node = CreateNode();
        ASSERT_TRUE(!IsNull(node));
        node->m_value = value;
        m_list.InsertAfter(iter.Node(), node);
        return Iterator_t(node);
    }

    Iterator_t Erase(Iterator_t iter) {
        Node_t* nextNode = m_list.Remove(iter.Node());
        DestoryNode(iter.Node());
        return Iterator_t(nextNode);
    }

    Iterator_t Find(const Value_t& value) {
        for (Iterator_t iter = Begin(); iter != End(); ++iter) {
            if (value == *iter) {
                return iter;
            }
        }
        return End();
    }

private:
    Node_t* CreateNode() {
        void* memory = m_allocator.Alloc();
        Node_t* node = new(memory) Node_t();
        return node;
    }

    void DestoryNode(Node_t* node) {
        node->~Node_t();
        m_allocator.Free(node);
    }

private:
    Allocator_t m_allocator;
    List_t m_list;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_UNIDLIST_HPP
