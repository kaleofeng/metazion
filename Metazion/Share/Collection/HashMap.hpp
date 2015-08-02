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
    using Key_t = KeyType;
    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Hasher_t = HasherType;
    using Allocator_t = AllocatorFamily;
    using Bucket_t = Map<Key_t, Value_t, Compare_t, Allocator_t>;
    using BucketIterator_t = typename Bucket_t::Iterator_t;
    using BucketConstIterator_t = typename Bucket_t::ConstIterator_t;

    class Iterator {
        friend class HashMap;

    public:
        Iterator() {}

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

        auto& operator *() {
            return m_iter.operator *();
        }

        auto* operator ->() {
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
        HashMap* m_owner = nullptr;
        int m_bucket = 0;
        BucketIterator_t m_iter;
    };

    class ConstIterator {
        friend class HashMap;

    public:
        ConstIterator() {}

        ConstIterator(const Iterator& other)
            : m_owner(other.m_owner)
            , m_bucket(other.m_bucket)
            , m_iter(other.m_iter) {}

        ConstIterator(const HashMap* owner, int bucket, BucketConstIterator_t iter)
            : m_owner(owner)
            , m_bucket(bucket)
            , m_iter(iter) {}

        ~ConstIterator() {}

        ConstIterator& operator =(const ConstIterator& other) {
            if (&other != this) {
                m_owner = other.m_owner;
                m_bucket = other.m_bucket;
                m_iter = other.m_iter;
            }
            return *this;
        }

        const auto& operator *() {
            return m_iter.operator *();
        }

        const auto* operator ->() {
            return m_iter.operator ->();
        }

        ConstIterator& operator ++() {
            const auto& bucket = m_owner->m_buckets[m_bucket];
            ++m_iter;
            if (m_iter != bucket.End()) {
                return *this;
            }

            for (m_bucket = m_bucket + 1; m_bucket < BUCKETSIZE; ++m_bucket) {
                const auto& nextBucket = m_owner->m_buckets[m_bucket];
                m_iter = nextBucket.Begin();
                if (m_iter != nextBucket.End()) {
                    return *this;
                }
            }

            *this = ConstIterator();
            return *this;
        }

        ConstIterator operator ++(int) {
            const auto temp = *this;
            const auto& bucket = m_owner->m_buckets[m_bucket];
            ++m_iter;
            if (m_iter != bucket.End()) {
                return temp;
            }

            for (m_bucket = m_bucket + 1; m_bucket < BUCKETSIZE; ++m_bucket) {
                const auto& nextBucket = m_owner->m_buckets[m_bucket];
                m_iter = nextBucket.Begin();
                if (m_iter != nextBucket.End()) {
                    return temp;
                }
            }

            *this = ConstIterator();
            return temp;
        }

        bool operator ==(const ConstIterator& other) const {
            if (IsNull(m_owner)) {
                return IsNull(other.m_owner);
            }

            return m_bucket == other.m_bucket && m_iter == other.m_iter;
        }

        bool operator !=(const ConstIterator& other) const {
            return !operator ==(other);
        }

    private:
        const HashMap* m_owner = nullptr;
        int m_bucket = 0;
        BucketConstIterator_t m_iter;
    };

public:
    using Iterator_t = Iterator;
    using ConstIterator_t = ConstIterator;

public:
    HashMap() {}

    ~HashMap() {}

    HashMap(const HashMap& other) {
        *this = other;
    }

    HashMap& operator =(const HashMap& other) {
        if (&other != this) {
            Clear();
            for (auto i = 0; i < BUCKETSIZE; ++i) {
                m_buckets[i] = other.m_buckets[i];
            }
            m_size = other.m_size;
        }
        return *this;
    }

    HashMap(HashMap&& other) {
        *this = std::move(other);
    }

    HashMap& operator =(HashMap&& other) {
        *this = other;
        return *this;
    }

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
    MZ_ADAPT_FOR_RANGE_TRAVERSAL()

    Iterator_t Begin() {
        if (!IsEmpty()) {
            for (auto index = 0; index < BUCKETSIZE; ++index) {
                auto& bucket = m_buckets[index];
                auto iter = bucket.Begin();
                if (iter != bucket.End()) {
                    return Iterator_t(this, index, iter);
                }
            }
        }
        
        return End();
    }

    ConstIterator_t Begin() const {
        if (!IsEmpty()) {
            for (auto index = 0; index < BUCKETSIZE; ++index) {
                const auto& bucket = m_buckets[index];
                const auto iter = bucket.Begin();
                if (iter != bucket.End()) {
                    return ConstIterator_t(this, index, iter);
                }
            }
        }

        return End();
    }

    Iterator_t End() {
        return Iterator_t();
    }

    ConstIterator_t End() const {
        return ConstIterator_t();
    }

    Iterator_t Insert(const Key_t& key, const Value_t& value) {
        const auto hashCode = m_hasher(key);
        const auto index = hashCode & (BUCKETSIZE - 1);
        auto& bucket = m_buckets[index];
        auto iter = bucket.Insert(key, value);
        if (iter != bucket.End()) {
            ++m_size;
        }

        return Iterator_t(this, index, iter);
    }

    Iterator_t Erase(Iterator_t iter) {
        MZ_ASSERT_TRUE(iter != End());
        MZ_ASSERT_TRUE(iter.m_owner == this);

        auto index = iter.m_bucket;
        auto& bucket = m_buckets[index];
        auto bucketIter = bucket.Erase(iter.m_iter);
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
        auto iter = bucket.Find(key);
        return Iterator_t(this, index, iter);
    }

private:
    Hasher_t m_hasher;
    Bucket_t m_buckets[BUCKETSIZE];
    int m_size = 0;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_HASHMAP_HPP_
