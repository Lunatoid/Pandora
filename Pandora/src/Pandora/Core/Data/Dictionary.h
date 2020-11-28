#pragma once

#include <memory>

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/Memory.h"
#include "Pandora/Core/Data/Hash.h"
#include "Pandora/Core/Data/Allocator.h"

namespace pd {

/// <summary>
/// Compares two keys by hashing them and comparing the hash.
/// </summary>
/// <param name="a">The first key.</param>
/// <param name="b">The seconds key.</param>
/// <returns>Whether or not the keys were equal.</returns>
template<typename K>
bool CompareKeys(K* a, K* b) {
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

    virtual ~Dictionary();
    
    /// <summary>
    /// Destructs all entries and frees the memory. This is called on destruction.
    /// </summary>
    void Delete();
    
    /// <summary>
    /// Checks if the dictionary contains the specified key.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether or not the key was found in the dictionary.</returns>
    bool Contains(const K* key);

    /// <summary>
    /// Checks if the dictionary contains the specified key.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether or not the key was found in the dictionary.</returns>
    inline bool Contains(const K& key);

    /// <summary>
    /// Gets the value associated with the key. Inserts the key if the value was not found.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>A reference to the value.</returns>
    V& Get(const K* key);

    /// <summary>
    /// Gets the value associated with the key. Inserts the key if the value was not found.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>A reference to the value.</returns>
    inline V& Get(const K& key);

    /// <summary>
    /// Sets the value assocaited with the key to <c>val</c>.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <param name="val">The value.</param>
    /// <param name="insertIfNotFound">Whether or not it should insert it if the key was not found.</param>
    /// <returns>Whether or not the value got set.</returns>
    inline bool Set(K* key, V* val, bool insertIfNotFound = true);

    /// <summary>
    /// Sets the value assocaited with the key to <c>val</c>.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <param name="val">The value.</param>
    /// <param name="insertIfNotFound">Whether or not it should insert it if the key was not found.</param>
    /// <returns>Whether or not the value got set.</returns>
    inline bool Set(const K& key, const V& val, bool insertIfNotFound = true);

    /// <summary>
    /// Inserts the key value pair into the dictionary.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <param name="val">The value.</param>
    /// <returns>Whether or not it inserted successfully.</returns>
    bool Insert(K* key, V* val);

    /// <summary>
    /// Inserts the key value pair into the dictionary.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <param name="val">The value.</param>
    /// <returns>Whether or not it inserted successfully.</returns>
    inline bool Insert(const K& key, const V& val);

    /// <summary>
    /// Removes the specified key value pair.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether or not it removed it successfully.</returns>
    bool Remove(const K* key);    

    /// <summary>
    /// Removes the specified key value pair.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether or not it removed it successfully.</returns>
    inline bool Remove(const K& key);

    /// <summary>
    /// Returns the raw data of the dictionary.
    /// </summary>
    /// <returns>The raw data of the dictionary.</returns>
    inline DictEntry<K, V>* Data() const;
    
    /// <summary>
    /// Returns how many bytes long the memory is.
    /// </summary>
    /// <returns>How many bytes long the memory is.</returns>
    inline u64 BufferSize() const;
    
    /// <summary>
    /// Returns how many key value pairs are in the dictionary.
    /// </summary>
    /// <returns>How many key value pairs are in the dictionary.</returns>
    inline int Count() const;
    
    /// <summary>
    /// Returns how many key value pairs can fit in the memory.
    /// </summary>
    /// <returns>How many key value pairs can fit in the memory.</returns>
    inline int Capacity() const;
    
    /// <summary>
    /// Returns what allocator the dictionary is using.
    /// </summary>
    /// <returns>What allocator the dictionary is using.</returns>
    inline Allocator Allocator() const ;

    V& operator[](const K& key);

    template<typename K, typename V>
    struct DictIt {
        DictIt(const Dictionary<K, V>& parent, int i) : parent(parent), i(i) {
            // If we pass in a value that is below zero that means we want to find
            // the first filled bucket
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

        bool operator==(const DictIt<K, V>& other) {
            return i == other.i && parent.Data() == other.parent.Data();
        }

        bool operator!=(const DictIt<K, V>& other) {
            return !operator==(other);
        }

    private:
        const Dictionary<K, V>& parent;
        int i;
    };

    DictIt<K, V> begin() const;
    DictIt<K, V> end() const;

protected:
    /// <summary>
    /// Initializes the dictionary. Should be called before accessing any memory.
    /// </summary>
    inline void Init();
    
    /// <summary>
    /// Doubles the memory and rehashes all key value pairs.
    /// </summary>
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

    u64 bucketCount = bufferSize / sizeof(Entry);
    for (int i = 0; i < bucketCount; i++) {
        if (Data()[i].dist >= 0) {
            Data()[i].key.~K();
            Data()[i].val.~V();
        }
    }

    Free(memory, allocator);

    bufferSize = 0;
    count = 0;
    memory = nullptr;
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Contains(const K* key) {
    Init();

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash((K*)key);
    u64 bucket = hash & (bucketCount - 1);

    bool looped = false;
    while (true) {
        Entry* entry = &memory[bucket];

        if (entry->dist < 0) {
            // Unsuccessful lookup
            return false;
        } else if (CompareKeys((K*)key, &entry->key)) {
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
inline bool Dictionary<K, V>::Contains(const K& key) {
    return Contains(&key);
}

template<typename K, typename V>
inline V& Dictionary<K, V>::Get(const K* key) {
    Init();

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash((K*)key);
    u64 bucket = hash & (bucketCount - 1);

    bool looped = false;
    while (true) {
        Entry* entry = &memory[bucket];

        if (entry->dist < 0) {
            Insert((K*)key, nullptr);
            return Get(key);
        } else if (CompareKeys((K*)key, &entry->key)) {
            return entry->val;
        } else if (looped) {
            Insert((K*)key, nullptr);
            return Get(key);
        }

        bucket += 1;

        if (bucket >= bucketCount) {
            looped = true;
            bucket = 0;
        }
    }

    Insert((K*)key, nullptr);
    return Get(key);
}

template<typename K, typename V>
inline V& Dictionary<K, V>::Get(const K& key) {
    return Get(&key);
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Set(K* key, V* val, bool insertIfNotFound) {
    Init();

    if (insertIfNotFound || Contains(key)) {
        Get(key) = *val;
        return true;
    }

    return false;
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Set(const K& key, const V& val, bool insertIfNotFound) {
    return Set((K*)&key, (V*)&val, insertIfNotFound);
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Insert(K* key, V* val) {
    Init();

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash((K*)key);
    u64 bucket = hash & (bucketCount - 1);

    const f32 REHASH_LOAD = 0.5f;

    Entry targetEntry;
    targetEntry.dist = 0;
    targetEntry.key = (K&)*key;

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
inline bool Dictionary<K, V>::Insert(const K& key, const V& val) {
    return Insert((K*)&key, (V*)&val);
}

template<typename K, typename V>
inline bool Dictionary<K, V>::Remove(const K* key) {
    Init();

    u64 bucketCount = bufferSize / sizeof(Entry);
    u64 hash = DoHash((K*)key);
    u64 bucket = hash & (bucketCount - 1);

    bool searchForKey = true;
    while (true) {
        Entry* entry = &memory[bucket];

        if (searchForKey && entry->dist >= 0 && CompareKeys(&entry->key, (K*)key)) {
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
inline bool Dictionary<K, V>::Remove(const K& key) {
    return Remove(&key);
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
inline Dictionary<K, V>::DictIt<K, V> Dictionary<K, V>::begin() const {
    return DictIt<K, V>(*this, -1);
}

template<typename K, typename V>
inline Dictionary<K, V>::DictIt<K, V> Dictionary<K, V>::end() const {
    return DictIt<K, V>(*this, (int)(bufferSize / sizeof(Entry)));
}

}
