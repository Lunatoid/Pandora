#pragma once

#include <memory>

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/Memory.h"
#include "Pandora/Core/Data/Hash.h"
#include "Pandora/Core/Data/Allocator.h"

namespace pd {

/**
 * \brief Compares to keys by hasing them and comparing the hash.
 *
 * \tparam K The key type.
 * \param a The first key.
 * \param b The second key.
 * \return Whether or not the hashes match.
 */
template<typename K>
bool CompareKeys(const K& a, const K& b) {
    return DoHash(a) == DoHash(b);
}


template<typename K, typename V>
struct DictEntry {
    static_assert(std::is_default_constructible<K>::value, "K must be default constructible.");
    static_assert(std::is_default_constructible<V>::value, "V must be default constructible.");

    i8 dist = -1;
    K key = K();
    V val = V();
};


template<typename K, typename V>
class Dictionary {
    using Entry = DictEntry<K, V>;
public:

    Dictionary(Allocator allocator = Allocator::Persistent) : allocator(allocator) {}
    Dictionary(const Dictionary<K, V>& other) : allocator(other.allocator) {
        operator=(other);
    }

    virtual ~Dictionary();

    /**
     * \brief Destructs all entries and frees the memory.
     * Gets called on destruction
     */
    void Delete();

    /**
     * \param key The key.
     * \return Whether or not the key was found in the dictionary.
     */
    bool Contains(const K& key) const;

    /**
     * \param key The key.
     * \return The value associated with the key. Inserts it if it wasn't found.
     */
    V& Get(const K& key);

    /**
     * \brief Sets the value assocaited with the key.
     *
     * \param key The key.
     * \param val The new value.
     * \param insertIfNotFound Whether or not it should insert the key-value pair if it wasn't found.
     * \return Whether or not the value got set.
     */
    bool Set(const K& key, const V& val, bool insertIfNotFound = true);

    /**
     * \brief Inserts a new key-value pair.
     *
     * \param key The key.
     * \param val The value.
     * \return Whether or not it inserted successfully.
     */
    bool Insert(const K& key, Optional<V> val = Optional<V>());

    /**
     * \brief Removes the specified key-value pair.
     *
     * \param key The key.
     * \return Whether or not it got removed successfully.
     */
    bool Remove(const K& key);

    /**
     * \return The raw data pointer of the dictionary.
     */
    inline DictEntry<K, V>* Data() const;

    /**
     * \return The size of the buffer in bytes.
     */
    inline u64 BufferSize() const;

    /**
     * \return How many key-value pairs are in the dictionary.
     */
    inline int Count() const;

    /**
     * \return How many key-value pairs can fit in the current buffer.
     */
    inline int Capacity() const;

    /**
     * \return The allocator used for the buffer.
     */
    inline Allocator Allocator() const;

    V& operator[](const K& key);

    Dictionary<K, V>& operator=(const Dictionary<K, V>& other) {
        if (other.Data() == Data()) return *this;

        // Delete and make buffer as big as `other`
        Delete();
        Init();

        while (Capacity() != other.Capacity()) {
            GrowAndRehash();
        }

        for (int i = 0; i < other.Capacity(); i++) {
            if (other.Data()[i].dist >= 0) {
                Data()[i] = other.Data()[i];
            }
        }

        return *this;
    }

    template<typename T, typename U>
    struct DictIt {
        DictIt(const Dictionary<T, U>& parent, int i) : parent(parent), i(i) {
            // If we pass in a iterator that is below zero that means we want to find
            // the first filled bucket, it's a little janky, maybe we want to use an Optional<int>
            if (i < 0) {
                operator++();
            }
        }

        const Entry& operator*() const {
            return parent.Data()[i];
        }

        void operator++() {
            do {
                i += 1;

                if (i >= parent.bufferSize / sizeof(Entry)) break;

            } while (parent.Data()[i].dist < 0);
        }

        bool operator==(const DictIt<T, U>& other) {
            return i == other.i && parent.Data() == other.parent.Data();
        }

        bool operator!=(const DictIt<T, U>& other) {
            return !operator==(other);
        }

    private:
        const Dictionary<T, U>& parent;
        int i;
    };

    DictIt<K, V> begin() const;
    DictIt<K, V> end() const;

protected:

    /**
     * \brief Initializes the dictionary. Should be called before accessing any memory.
     *
     */
    inline void Init();

    /**
     * \brief Doubles the buffer sizes and reinserts all the key-value pairs.
     */
    void GrowAndRehash();

    pd::Allocator allocator = Allocator::None;

    Entry* memory = nullptr;
    u64 bufferSize = 0;
    int count = 0;
};

//
// Implementation
//

template<typename K, typename V>
inline Dictionary<K, V>::~Dictionary() {
    Delete();
}

template<typename K, typename V>
inline void Dictionary<K, V>::Delete() {
    if (!Data()) return;

    for (auto& entry : *this) {
        if (entry.dist >= 0) {
            entry.key.~K();
            entry.val.~V();
        }
    }

    Free(memory, allocator);

    bufferSize = 0;
    count = 0;
    memory = nullptr;
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Contains(const K& key) const {
    if (!Data()) return false;

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash(key);
    u64 bucket = hash & (bucketCount - 1);

    bool looped = false;
    while (true) {
        Entry* entry = &memory[bucket];

        if (entry->dist < 0) {
            // Unsuccessful lookup
            return false;
        } else if (CompareKeys(key, entry->key)) {
            return true;
        } else if (looped) {
            // Unsuccessful lookup
            return false;
        }

        bucket += 1;

        if (bucket >= bucketCount) {
            looped = true;
            bucket = 0;
        }
    }

    return false;
}

template<typename K, typename V>
inline V& Dictionary<K, V>::Get(const K& key) {
    Init();

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash(key);
    u64 bucket = hash & (bucketCount - 1);
    u64 idealBucket = bucket;

    bool looped = false;
    while (true) {
        Entry* entry = &memory[bucket];

        if (entry->dist < 0) {
            Insert(key);
            return Get(key);
        } else if (CompareKeys(key, entry->key)) {
            return entry->val;
        } else if (looped) {
            // If we looped and we're back at the bucket we started at,
            // that means we have searched every possible spot.
            if (bucket == idealBucket) {
                Insert(key);
                return Get(key);
            }
        }

        bucket += 1;

        if (bucket >= bucketCount) {
            looped = true;
            bucket = 0;
        }
    }

    Insert(key);
    return Get(key);
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Set(const K& key, const V& val, bool insertIfNotFound) {
    Init();

    if (insertIfNotFound || Contains(key)) {
        Get(key) = val;
        return true;
    }

    return false;
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Insert(const K& key, Optional<V> val) {
    Init();

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash(key);
    u64 bucket = hash & (bucketCount - 1);

    const f32 REHASH_LOAD = 0.5f;

    Entry targetEntry;
    targetEntry.dist = 0;
    targetEntry.key = key;

    if (val) {
        targetEntry.val = *val;
    }

    while (true) {
        Entry* entry = &memory[bucket];

        if (entry->dist < 0) {
            new (entry) Entry();
            *entry = targetEntry;
            count += 1;

            // Check for rehash
            int rehashThreshold = (int)(f32(Capacity()) * REHASH_LOAD);
            if (count > rehashThreshold) {
                GrowAndRehash();
            }

            return true;
        } else {
            if (targetEntry.dist > entry->dist) {
                // Steal the spot and search for the new target
                Entry& tmp = memory[bucket];

                targetEntry = memory[bucket];
                memory[bucket] = tmp;
            }
        }

        bucket += 1;
        targetEntry.dist += 1;

        if (bucket >= bucketCount) {
            bucket = 0;
        }
    }

    return false;
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Remove(const K& key) {
    Init();

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash(key);
    u64 bucket = hash & (bucketCount - 1);

    bool searchForKey = true;
    while (true) {
        Entry* entry = &memory[bucket];

        if (searchForKey && entry->dist >= 0 && CompareKeys(entry->key, key)) {
            // Clear slot
            entry->dist = -1;
            entry->key.~K();
            entry->val.~V();

            count -= 1;
            searchForKey = false;
        } else if (!searchForKey) {
            if (entry->dist > 0) {
                memory[bucket - 1] = memory[bucket];
                memory[bucket - 1].dist -= 1;
            } else {
                memory[bucket - 1].dist = -1;
                return true;
            }
        }

        bucket += 1;

        if (bucket >= bucketCount) {
            bucket = 0;
        }
    }

    return false;
}

template<typename K, typename V>
inline DictEntry<K, V>* Dictionary<K, V>::Data() const {
    return memory;
}

template<typename K, typename V>
inline u64 Dictionary<K, V>::BufferSize() const {
    return bufferSize;
}

template<typename K, typename V>
inline int Dictionary<K, V>::Count() const {
    return count;
}

template<typename K, typename V>
inline int Dictionary<K, V>::Capacity() const {
    return (int)(BufferSize() / sizeof(Entry));
}

template<typename K, typename V>
inline Allocator Dictionary<K, V>::Allocator() const {
    return allocator;
}

template<typename K, typename V>
inline V& Dictionary<K, V>::operator[](const K& key) {
    return Get(key);
}

template<typename K, typename V>
inline void Dictionary<K, V>::Init() {
    const int INITIAL_LENGTH = 32;

    if (!memory) {
        if (bufferSize == 0) {
            bufferSize = INITIAL_LENGTH * sizeof(Entry);
        }

        memory = (DictEntry<K, V>*)Alloc(bufferSize, allocator);
        MemorySet(memory, bufferSize, -1);
    }
}

template<typename K, typename V>
inline void Dictionary<K, V>::GrowAndRehash() {
    Init();

    int oldBucketCount = Capacity();

    Entry* oldMemory = memory;
    bufferSize *= 2; // Keep power of 2
    memory = (Entry*)Alloc(bufferSize, allocator);
    MemorySet(memory, bufferSize, -1);

    // Insert old keys
    for (int i = 0; i < oldBucketCount; i++) {
        if (oldMemory[i].dist >= 0) {
            Insert(oldMemory[i].key, oldMemory[i].val);
        }
    }

    // Delete old keys
    for (int i = 0; i < oldBucketCount; i++) {
        if (oldMemory[i].dist >= 0) {
            oldMemory[i].key.~K();
            oldMemory[i].val.~V();
        }
    }

    if (allocator == Allocator::Persistent) {
        Free(oldMemory);
    }
}

template<typename K, typename V>
inline typename Dictionary<K, V>::template DictIt<K, V> Dictionary<K, V>::begin() const {
    return DictIt<K, V>(*this, -1);
}

template<typename K, typename V>
inline typename Dictionary<K, V>::template DictIt<K, V> Dictionary<K, V>::end() const {
    return DictIt<K, V>(*this, (int)(bufferSize / sizeof(Entry)));
}

}
