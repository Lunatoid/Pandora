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

    /**
     * \brief Calls `Copy()` on the other reference.
     * 
     * \param other The other reference.
     */
    Ref(const Ref<T>& other);

    /**
     * \brief Calls `Reset` with `data`.
     * 
     * \param data The data.
     */
    Ref(T* data);

    ~Ref();

    /**
     * \brief Constructs a new reference with the persistent allocator.
     * 
     * \param args The constructor arguments.
     * \return The new reference.
     */
    template<typename ...Args>
    static Ref<T> Create(Args ...args);

    /**
     * \brief Creates a new reference based on this reference.
     * 
     * \param type The type of reference to create.
     * \return The new reference.
     */
    Ref<T> NewRef(RefType type = RefType::Strong) const;

    /**
     * \brief Force deletes the reference despite how many references it still holds.
     */
    void Delete();

    /**
     * \brief Resets the reference.
     * If `newData` is not nullptr it will initialize it with the new data.
     * 
     * \param newData Optional new data. Data must be allocated with the persistent allocator.
     */
    void Reset(T* newData = nullptr);

    /**
     * \brief Changes the reference type.
     * 
     * \param newType The new type.
     */
    void ChangeType(RefType newType);

    /**
     * \return The current reference type.
     */
    RefType Type() const;

    /**
     * \param type Which count to get.
     * \return The reference count.
     */
    int Count(RefType type) const;

    /**
     * \return The raw data pointer.
     */
    T* Get() const;

    /**
     * \tparam U The type to cast the raw data pointer as.
     * \return The casted raw data.
     */
    template<typename U>
    U* As();

    Ref<T>& operator=(const Ref<T>& other);

    T* operator->() const;

    T& operator*() const;

    operator bool() const;

private:

    /**
     * \brief Copies the reference, increasing the specified reference count.
     * 
     * \param other The other reference.
     */
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
inline Ref<T>::Ref(const Ref<T>& other) {
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
inline Ref<T> Ref<T>::NewRef(RefType type) const {
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
inline T& Ref<T>::operator*() const {
    return *Get();
}

template<typename T>
inline Ref<T>::operator bool() const {
    return Get() != nullptr;
}

}
