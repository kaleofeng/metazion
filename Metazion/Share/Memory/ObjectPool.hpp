#ifndef _MZ_SHARE_OBJECTPOOL_HPP_
#define _MZ_SHARE_OBJECTPOOL_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/UDList.hpp"
#include "Metazion/Share/Collection/UDSelfList.hpp"
#include "Metazion/Share/Memory/StepAllocator.hpp"
#include "Metazion/Share/Sync/NoneLock.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename ObjectType
, typename AllocatorFamily = StepAllocator<64>
, typename LockType = NoneLock
>
class ObjectPool {
    DISALLOW_COPY_AND_ASSIGN(ObjectPool)
    
    using Object_t = ObjectType;
    using Lock_t = LockType;
    using Allocator_t = typename AllocatorFamily::template Rebind<sizeof(Object_t)>;

public:
    ObjectPool() { m_allocator.Initialize(); }

    ~ObjectPool() { m_allocator.Finalize(); }

public:
    Object_t* Obtain() {
        std::lock_guard<Lock_t> lockGuard(m_lock);
        
        return CreateObject();
    }

    void Return(Object_t* object) {
        std::lock_guard<Lock_t> lockGuard(m_lock);

        DestoryObject(object);
    }

private:
    Object_t* CreateObject() {
        auto memory = m_allocator.Alloc();
        auto object = new(memory) Object_t();
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
    using Object_t = ObjectType;
    using ObjectPool_t = ObjectPool<Object_t, AllocatorFamily, LockType>;

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

#endif // _MZ_SHARE_OBJECTPOOL_HPP_
