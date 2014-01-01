#ifndef MZ_SHARE_OBJECTPOOL_HPP
#define MZ_SHARE_OBJECTPOOL_HPP

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Memory/StepAllocator.hpp"
#include "Metazion/Share/Collection/UnidList.hpp"
#include "Metazion/Share/Collection/UnidSelfList.hpp"
#include "Metazion/Share/Sync/NoneLock.hpp"
#include "Metazion/Share/Sync/AutoGuard.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ObjectType
, typename AllocatorFamily = StepAllocator<64>
, typename LockType = NoneLock
>
class ObjectPool {
    DISALLOW_COPY_AND_ASSIGN(ObjectPool)
    
    typedef ObjectType Object_t;
    typedef LockType Lock_t;
    typedef typename AllocatorFamily::template
        Rebind<sizeof(Object_t)>::Allocator_t Allocator_t;

public:
    ObjectPool() { m_allocator.Initialize(); }

    ~ObjectPool() { m_allocator.Finalize(); }

public:
    Object_t* Obtain() {
        AutoGuard<Lock_t> autoGuard(m_lock);
        
        return CreateObject();
    }

    void Return(Object_t *object) {
        AutoGuard<Lock_t> autoGuard(m_lock);

        DestoryObject(object);
    }

private:
    Object_t* CreateObject() {
        void* memory = m_allocator.Alloc();
        Object_t* object = new(memory) Object_t();
        return object;
    }

    void DestoryObject(Object_t* object) {
        object->~Object_t();
        m_allocator.Free(object);
    }

private:
    Allocator_t m_allocator;
    Lock_t m_lock;
};

template<typename ObjectType
, typename AllocatorFamily = StepAllocator<64>
, typename LockType = NoneLock
>
class PortableObjectPool {
    typedef ObjectType Object_t;
    typedef ObjectPool<Object_t, AllocatorFamily, LockType> ObjectPool_t;

public:
    static Object_t* Obtain() {
        return GetObjectPool().Obtain();
    }

    static void Return(Object_t* object) {
        GetObjectPool().Return(object);
    }

private:
    static ObjectPool_t& GetObjectPool() {
        static ObjectPool_t s_objectPool;
        return s_objectPool;
    }
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_SHARE_OBJECTPOOL_HPP
