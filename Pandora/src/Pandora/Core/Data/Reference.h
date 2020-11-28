#pragma once

#include "Pandora/Core/Data/Allocator.h"
#include "Pandora/Core/Assert.h"

namespace pd {

/// <summary>
/// <c>Strong</c> references control the lifespan of the reference. If this is 0 it will <c>Delete()</c> itself.
/// <c>Weak</c> reference don't control the lifespan.
/// </summary>
enum class RefType : byte {
    Strong,
    Weak
};

template<typename T>
class Ref {
public:
    Ref() = default;
    
    /// <summary>
    /// Calls <c>Copy()</c> with <c>other</c>.
    /// </summary>
    /// <param name="other">The other reference.</param>
    Ref(Ref<T>& other);
    
    /// <summary>
    /// Calls <c>Reset()</c> with <c>data</c>.
    /// </summary>
    /// <param name="data">The data.</param>
    Ref(T* data);

    ~Ref();

    /// <summary>
    /// Allocates a new reference using the persistent allocator.
    /// </summary>
    /// <param name="args">The constructor arguments to use.</param>
    /// <returns>The new reference.</returns>
    template<typename ...Args>
    static Ref<T> Create(Args ...args);
    
    /// <summary>
    /// Creates a new reference based on the current one with the desired type.
    /// </summary>
    /// <param name="type">The type.</param>
    /// <returns>A new reference with the desired type.</returns>
    Ref<T> NewRef(RefType type = RefType::Strong);

    /// <summary>
    /// Delets the reference data despite how many references it still has.
    /// </summary>
    void Delete();

    /// <summary>
    /// Resets the reference. If <c>newData != nullptr</c> it will initialize it with new data.
    /// </summary>
    /// <param name="newData">The new data to set it to. Can be <c>nullptr</c>. Must be allocated with <c>Allocator::New</c>.</param>
    void Reset(T* newData = nullptr);

    /// <summary>
    /// Changes the type of the reference.
    /// </summary>
    /// <param name="newType">The new type.</param>
    void ChangeType(RefType newType);

    /// <summary>
    /// Returns the reference type.
    /// </summary>
    /// <returns>The reference type.</returns>
    RefType Type() const;

    /// <summary>
    /// Gets the reference count for the specified type.
    /// </summary>
    /// <param name="type">The type.</param>
    /// <returns>The reference count for the specified type.</returns>
    int Count(RefType type) const;

    /// <summary>
    /// Returns the raw pointer to the data.
    /// </summary>
    /// <returns>The raw pointer to the data.</returns>
    T* Get() const;

    /// <summary>
    /// Returns the raw pointer to the data casted as <c>U</c>.
    /// </summary>
    /// <returns>The raw pointer to the data casted as <c>U</c>.</returns>
    template<typename U>
    U* As();

    Ref<T>& operator=(const Ref<T>& other);

    T* operator->() const;

    T& operator*();
    
    operator bool();

private:
    /// <summary>
    /// Copies the reference, increasing the reference count for the desired type.
    /// </summary>
    /// <param name="other">The other reference.</param>
    void Copy(const Ref<T>& other) {
        // If you do something like `ref = ref;` we don't want to increase the count
        // This can lead to things having an infinite lifespan. Bad!
        if (data == other.data) return;

        DecCount(type);

        data = other.data;
        count = other.count;
        type = other.type;

        IncCount(type);
    }

    inline void IncCount(RefType type) {
        if (!count) return;

        if (type == RefType::Strong) {
            count->strong += 1;
        } else {
            count->weak += 1;
        }
    }

    inline void DecCount(RefType type) {
        if (!count) return;

        if (type == RefType::Strong) {
            count->strong -= 1;

            if (count->strong <= 0) {
                PD_ASSERT_D(count->strong == 0, "negative reference count (strong: %d)", count->strong);
                Delete();
            }
        } else {
            count->weak -= 1;
        }
    }

    struct RefCount {
        RefCount() = default;
        RefCount(int strong, int weak) :
            strong(strong), weak(weak) {}

        int strong = 0;
        int weak = 0;
    };

    RefCount* count = nullptr;
    RefType type = RefType::Weak;
    T* data = nullptr;
};

template<typename T>
template<typename ...Args>
inline Ref<T> Ref<T>::Create(Args ...args) {
    Ref<T> ref;
    ref.data = New<T>(args...);
    ref.type = RefType::Strong;
    ref.count = New<Ref::RefCount>(1, 0);

    return ref;
}

template<typename T>
template<typename U>
inline U* Ref<T>::As() {
    return (U*)Get();
}

template<typename T>
inline Ref<T>::Ref(Ref<T>& other) {
    Copy(other);
}

template<typename T>
inline Ref<T>::Ref(T* data) {
    Reset(data);
}

template<typename T>
inline Ref<T>::~Ref() {
    Reset();
}

template<typename T>
inline Ref<T> Ref<T>::NewRef(RefType type) {
    Ref<T> newRef;
    newRef.type = type;
    newRef.data = data;
    newRef.count = count;

    if (count) {
        if (type == RefType::Strong) {
            ++newRef.count->strong;
        } else {
            ++newRef.count->weak;
        }
    }

    return newRef;
}

template<typename T>
inline void Ref<T>::Delete() {
    if (data) {
        pd::Delete(data);
        data = nullptr;
    }

    if (count) {
        pd::Delete(count);
        count = nullptr;
    }
}

template<typename T>
inline void Ref<T>::Reset(T* newData) {
    DecCount(type);

    if (newData) {
        if (!count) {
            count = New<RefCount>(1, 0);
        } else {
            count->strong = 1;
            count->weak = 0;
        }

        data = newData;
        type = RefType::Strong;
    } else {
        data = nullptr;
        count = nullptr;
    }
}

template<typename T>
inline void Ref<T>::ChangeType(RefType newType) {
    if (newType == type) return;

    if (newType == RefType::Strong) {
        DecCount(RefType::Weak);
        IncCount(RefType::Strong);
    } else {
        DecCount(RefType::Strong);
        IncCount(RefType::Weak);
    }

    type = newType;
}

template<typename T>
inline RefType Ref<T>::Type() const {
    return type;
}

template<typename T>
inline int Ref<T>::Count(RefType type) const {
    if (!count) return 0;

    if (type == RefType::Strong) {
        return count->strong;
    } else {
        return count->weak;
    }
}

template<typename T>
inline T* Ref<T>::Get() const {
    return data;
}

template<typename T>
inline Ref<T>& Ref<T>::operator=(const Ref<T>& other) {
    Copy(other);
    return *this;
}

template<typename T>
inline T* Ref<T>::operator->() const {
    return Get();
}

template<typename T>
inline T& Ref<T>::operator*() {
    return *Get();
}

template<typename T>
inline Ref<T>::operator bool() {
    return data != nullptr;
}

}
