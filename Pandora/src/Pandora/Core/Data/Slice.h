#pragma once

#include "Pandora/Core/Data/Allocator.h"

#include <cstring>

namespace pd {

template<typename T>
class Array;

template <typename T>
class Slice {
public:
    Slice() = default;
    Slice(const T* memory, int count) : memory((T*)memory), count(count) {}
    Slice(const char* str) : memory((T*)str), count((int)strlen(str) / sizeof(T)) {}
    Slice(Array<T>& array);

    /**
     * \return The raw data pointer of the slice.
     */
    inline T* Data() const;

    /**
     * \return The size of the slice in bytes.
     */
    virtual inline u64 SizeInBytes() const;

    /**
     * \return How many elements are in the slice.
     */
    inline int Count() const;

    /**
     * \brief Casts the slice data as a new slice.
     * 
     * \tparam U The new type to cast it as.
     * \return The casted slice.
     */
    template<typename U>
    inline Slice<U> As() const;

    /**
     * \brief Appends the data of this slice to the array.
     * 
     * \param out The output array.
     */
    inline void ToArray(Array<T>& out);

    /**
     * \param index The index.
     * \return The element at the specified index.
     */
    const T& At(int index) const;

    const T& operator[](int index) const;

    template<typename U>
    struct SliceIt {
        SliceIt(const Slice<U>& parent, int i) : parent(parent), i(i) {}

        const T& operator*() const {
            return parent.At(i);
        }

        void operator++() {
            i += 1;
        }

        bool operator==(const SliceIt<U>& other) {
            return i == other.i && parent.Data() == other.parent.Data();
        }

        bool operator!=(const SliceIt<U>& other) {
            return !operator==(other);
        }

    private:
        int i;
        const Slice<U>& parent;
    };

    SliceIt<T> begin() const;
    SliceIt<T> end() const;

protected:
    T* memory = nullptr;
    int count = 0;
};

template<typename T>
inline Slice<byte> ToBytes(T* t) {
    return Slice<byte>((byte*)t, sizeof(*t));
}

//
// Implementation
//

template<typename T>
inline Slice<T>::Slice(Array<T>& array) : memory(array.Data()), count(array.Count()) {}

template<typename T>
inline T* Slice<T>::Data() const {
    return memory;
}

template<typename T>
inline u64 Slice<T>::SizeInBytes() const {
    return count * sizeof(T);
}

template<typename T>
inline int Slice<T>::Count() const {
    return count;
}

template<typename T>
inline void Slice<T>::ToArray(Array<T>& out) {
    out.AddRange(*this);
}

template<typename T>
inline const T& Slice<T>::At(int index) const {
    return Data()[index];
}

template<typename T>
const T& Slice<T>::operator[](int index) const {
    return At(index);
}

template<typename T>
inline typename Slice<T>::template SliceIt<T> Slice<T>::begin() const {
    return SliceIt<T>(*this, 0);
}

template<typename T>
inline typename Slice<T>::template SliceIt<T> Slice<T>::end() const {
    return SliceIt<T>(*this, Count());
}

template<typename T>
template<typename U>
inline Slice<U> Slice<T>::As() const {
    return Slice<U>((U*)Data(), Count() * sizeof(T) / sizeof(U));
}

}
