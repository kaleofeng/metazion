#ifndef _MZ_SHARE_HASHMAP_HPP_
#define _MZ_SHARE_HASHMAP_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include "Metazion/Share/Collection/Hasher.hpp"
#include "Metazion/Share/Collection/Map.hpp"
#include "Metazion/Share/Memory/HeapAllocator.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

template<typename KeyType
, typename ValueType
, typename CompareType = LessCompare<KeyType>
, typename HasherType = IntegerHasher<KeyType>
, int BUCKETSIZE = 1024
, typename AllocatorFamily = HeapAllocator<>
>
class HashMap {
    DISALLOW_COPY_AND_ASSIGN(HashMap)
    
    using Key_t = KeyType;
    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Hasher_t = HasherType;
    using Allocator_t = AllocatorFamily;
    using Bucket_t = Map<Key_t, Value_t, Compare_t, Allocator_t>;
    using BucketEntry_t = Pair<Key_t, Value_t>;
    using BucketIterator_t = typename Bucket_t::Iterator_t;

    class Iterator {
        friend class HashMap;

    public:
        Iterator()
            : m_owner(nullptr)
            , m_bucket(0) {}

        Iterator(const Iterator& other)
            : m_owner(other.m_owner)
            , m_bucket(other.m_bucket)
            , m_iter(other.m_iter) {}

        Iterator(HashMap* owner, int bucket, BucketIterator_t iter)
            : m_owner(owner)
            , m_bucket(bucket)
            , m_iter(iter) {}

        ~Iterator() {}

        Iterator& operator =(const Iterator& other) {
            if (&other != this) {
                m_owner = other.m_owner;
                m_bucket = other.m_bucket;
                m_iter = other.m_iter;
            }
            return *this;
        }

        BucketEntry_t& operator *() {
            return m_iter.operator *();
        }

        BucketEntry_t* operator ->() {
            return m_iter.operator ->();
        }

        Iterator& operator ++() {
            auto& bucket = m_owner->m_buckets[m_bucket];
            ++m_iter;
            if (m_iter != bucket.End()) {
                return *this;
            }

            for (m_bucket = m_bucket + 1; m_bucket < BUCKETSIZE; ++m_bucket) {
                auto& nextBucket = m_owner->m_buckets[m_bucket];
                m_iter = nextBucket.Begin();
                if (m_iter != nextBucket.End()) {
                    return *this;
                }
            }
            
            *this = Iterator();
            return *this;
        }

        Iterator operator ++(int) {
            auto temp = *this;
            auto& bucket = m_owner->m_buckets[m_bucket];
            ++m_iter;
            if (m_iter != bucket.End()) {
                return temp;
            }

            for (m_bucket = m_bucket + 1; m_bucket < BUCKETSIZE; ++m_bucket) {
                auto& nextBucket = m_owner->m_buckets[m_bucket];
                m_iter = nextBucket.Begin();
                if (m_iter != nextBucket.End()) {
                    return temp;
                }
            }

            *this = Iterator();
            return temp;
        }

        bool operator ==(const Iterator& other) const {
            if (IsNull(m_owner)) {
                return IsNull(other.m_owner);
            }

            return m_bucket == other.m_bucket && m_iter == other.m_iter;
        }

        bool operator !=(const Iterator& other) const {
            return !operator ==(other);
        }

    private:
        HashMap* m_owner;
        int m_bucket;
        BucketIterator_t m_iter;
    };

public:
    using Iterator_t = Iterator;

public:
    HashMap()
        : m_size(0) {}

    ~HashMap() {}

public:
    void Clear() {
        for (auto index = 0; index < BUCKETSIZE; ++index) {
            m_buckets[index].Clear();
        }
        m_size = 0;
    }

    bool IsEmpty() const {
        return m_size == 0;
    }

    int GetSize() const {
        return m_size;
    }

public:
    ADAPT_FOR_RANGE_TRAVERSAL()

    Iterator_t Begin() {
        if (!IsEmpty()) {
            for (auto index = 0; index < BUCKETSIZE; ++index) {
                auto& bucket = m_buckets[index];
                BucketIterator_t iter = bucket.Begin();
                if (iter != bucket.End()) {
                    return Iterator_t(this, index, iter);
                }
            }
        }
        
        return End();
    }

    Iterator_t End() {
        return Iterator_t();
    }

    Iterator_t Insert(const Key_t& key, const Value_t& value) {
        const auto hashCode = m_hasher(key);
        const auto index = hashCode & (BUCKETSIZE - 1);
        auto& bucket = m_buckets[index];
        BucketIterator_t iter = bucket.Insert(key, value);
        ++m_size;
        return Iterator_t(this, index, iter);
    }

    Iterator_t Erase(Iterator_t iter) {
        ASSERT_TRUE(iter != End());
        ASSERT_TRUE(iter.m_owner == this);

        auto index = iter.m_bucket;
        auto& bucket = m_buckets[index];
        BucketIterator_t bucketIter = bucket.Erase(iter.m_iter);
        --m_size;

        if (bucketIter != bucket.End()) {
            return Iterator_t(this, index, bucketIter); 
        }

        for (index = index + 1; index < BUCKETSIZE; ++index) {
            auto& nextBucket = m_buckets[index];
            bucketIter = nextBucket.Begin();
            if (bucketIter != nextBucket.End()) {
                return Iterator_t(this, index, bucketIter);
            }
        }

        return End();
    }

    Iterator_t Find(const Key_t& key) {
        const auto hashCode = m_hasher(key);
        const auto index = hashCode & (BUCKETSIZE - 1);
        auto& bucket = m_buckets[index];
        BucketIterator_t iter = bucket.Find(key);
        return Iterator_t(this, index, iter);
    }

private:
    Hasher_t m_hasher;
    Bucket_t m_buckets[BUCKETSIZE];
    int m_size;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_HASHMAP_HPP_
