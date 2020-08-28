#pragma once

#include "Pandora/Core/Data/Allocator.h"

namespace pd {

template<typename T>
class Array;

template <typename T>
class Slice {
public:
    Slice() = default;
    Slice(T* memory, int count) : memory(memory), count(count) {}
    Slice(const char* str) : memory((T*)str), count((int)strlen(str) / sizeof(T)) {}
    Slice(Array<T>& array);

    /// <summary>
    /// </summary>
    /// <returns>The raw data of the slice.</returns>
    inline T* Data() const;

    /// <summary>
    /// </summary>
    /// <returns>How many bytes long the slice is.</returns>
    virtual inline u64 SizeInBytes() const;

    /// <summary>
    /// </summary>
    /// <returns>How many elements are in the slice.</returns>
    inline int Count() const;

    /// <summary>
    /// Reinterpets the data of the slice as <c>U</c>
    /// </summary>
    /// <returns>The reinterpreted slice.</returns>
    template<typename U>
    inline Slice<U> As() const;

    /// <summary>
    /// Creates an array based on this slice.
    /// </summary>
    /// <param name="allocator">The allocator to use.</param>
    /// <returns>The array</returns>
    inline Array<T>& ToArray(Allocator allocator = Allocator::Persistent);

    /// <summary>
    /// Returns the element at the specified index. This operation is O(1).
    /// </summary>
    /// <param name="index">The index.</param>
    /// <returns>The element at that index.</returns>
    const T& At(int index);

    const T& operator[](int index);

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
inline Array<T>& Slice<T>::ToArray(Allocator allocator) {
    Array<T> array(allocator);
    array.AddRange(*this);
    return array;
}

template<typename T>
inline const T& Slice<T>::At(int index) {
    return Data()[index];
}

template<typename T>
const T& Slice<T>::operator[](int index) {
    return At(index);
}

template<typename T>
template<typename U>
inline Slice<U> Slice<T>::As() const {
    return Slice<U>((U*)Data(), Count() * sizeof(T) / sizeof(U));
}

}
