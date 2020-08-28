#pragma once

#include <memory.h>
#include <new>

#include "Pandora/Core/Assert.h"
#include "Pandora/Core/Types.h"

#include "Pandora/Core/Data/Allocator.h"
#include "Pandora/Core/Data/Memory.h"
#include "Pandora/Core/Data/Slice.h"

#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

template<typename T>
class Array {
public:
    /// <summary>
    /// A new array.
    /// </summary>
    /// <param name="allocator">The allocator.</param>
    Array(Allocator allocator = Allocator::Persistent);

    virtual ~Array();
    
    /// <summary>
    /// Destructs all elements and frees the memory. This is called on destruction.
    /// </summary>
    void Delete();
    
    /// <summary>
    /// Resizes the memory to hold <c>newCount</c> elements.
    /// If <c>newCount</c> is larger than the current count it will grow the memory.
    /// If <c>newCount</c> is smaller than the current count it will destruct the excess elements.
    /// </summary>
    /// <param name="newCount">The new count.</param>
    void Resize(int newCount);
    
    /// <summary>
    /// Adds a new elements to the end of the array.
    /// </summary>
    /// <param name="t">The new element.</param>
    /// <returns>The index of the element.</returns>
    int Add(const T& t);

    /// <summary>
    /// Adds a new elements to the end of the array.
    /// </summary>
    /// <param name="t">The new element.</param>
    /// <returns>The index of the element.</returns>
    int Add(const T* t);

    /// <summary>
    /// Instantiates a new element at the end of the array.
    /// </summary>
    /// <param name="...args">The constructor arguments.</param>
    /// <returns>The index of the element.</returns>
    template<typename ...Args>
    int Add(Args ...args);
    
    /// <summary>
    /// Adds a range of elements to the end of the array.
    /// </summary>
    /// <param name="memory">The memory.</param>
    /// <param name="count">The count.</param>
    /// <returns>The starting index of the new elements.</returns>
    int AddRange(T* memory, int count);

    /// <summary>
    /// Adds a range of elements to the end of the array.
    /// </summary>
    /// <param name="elements">The elements.</param>
    /// <returns>The starting index of the new elements.</returns>
    int AddRange(const Slice<T>& elements);
    
    /// <summary>
    /// Inserts an element into the specified index. This operation is ordered.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="t">The element.</param>
    void Insert(int index, const T& t);

    /// <summary>
    /// Inserts an element into the specified index. This operation is ordered.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="t">The element.</param>
    void Insert(int index, const T* t);

    /// <summary>
    /// Inserts an element into the specified index. This operation is unordered.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="t">The element.</param>
    void InsertUnordered(int index, const T& t);

    /// <summary>
    /// Inserts an element into the specified index. This operation is unordered.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="t">The element.</param>
    void InsertUnordered(int index, const T* t);
    
    /// <summary>
    /// Swaps two elements
    /// </summary>
    /// <param name="a">The first index to swap.</param>
    /// <param name="b">The seconds index to swap.</param>
    void Swap(int a, int b);
    
    /// <summary>
    /// Resizes the memory and constructs <c>reservationCount</c> elements. Also increases the count.
    /// </summary>
    /// <param name="reservationCount">How many elements to reserve.</param>
    void Reserve(int reservationCount);
    
    /// <summary>
    /// Removes the element at the specified index. This operation is ordered.
    /// </summary>
    /// <param name="index">The index.</param>
    void Remove(int index);

    /// <summary>
    /// Removes the element at the specified index. This operation is unordered.
    /// </summary>
    /// <param name="index">The index.</param>
    void RemoveUnordered(int index);

    /// <summary>
    /// Removes a range of elements. This operation is ordered.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="index">The count.</param>
    void RemoveRange(int index, int count);
    
    /// <summary>
    /// Destructs all elements and resets the count.
    /// </summary>
    inline void Clear();
    
    /// <summary>
    /// Uses quicksort to sort <c>count</c> elements starting from <c>offset</c>.
    /// </summary>
    /// <param name="func">The sort function. Needs to return <c>true</c> if <c>a</c> is less than <c>b</c>.</param>
    /// <param name="offset">The offset.</param>
    /// <param name="count">The count. -1 for remaining elements.</param>
    template<typename LessLambda>
    void Sort(LessLambda func, int offset = 0, int count = -1);
    
    /// <summary>
    /// Searches through <c>count</c> elements starting from <c>offset</c>.
    /// </summary>
    /// <param name="func">The find function. Returns true if the elements are equal.</param>
    /// <param name="context">The context to pass to <c>func</c>.</param>
    /// <param name="offset">The offset.</param>
    /// <param name="count">The count. -1 for remaining elements.</param>
    /// <returns>The index of the element</returns>
    template<typename FindLambda>
    int Find(FindLambda func, int offset = 0, int count = -1);

    /// <summary>
    /// Changes the allocator of the string.
    /// If changed to <c>Allocator::None</c> then <c>Delete()</c> is called.
    /// Otherwise, the existing memory is copied to the new memory and the original memory is freed (if necessary).
    /// </summary>
    /// <param name="allocator">The allocator.</param>
    virtual void ChangeAllocator(pd::Allocator allocator);
    
    /// <summary>
    /// Returns the element at the specified index. This operation is O(1).
    /// </summary>
    /// <param name="index">The index.</param>
    /// <returns>The element at that index.</returns>
    inline T& At(int index);
    
    /// <summary>
    /// </summary>
    /// <returns>The first element.</returns>
    inline T& First();

    /// <summary>
    /// </summary>
    /// <returns>The last element.</returns>
    inline T& Last();

    /// <summary>
    /// </summary>
    /// <returns>The raw data of the array.</returns>
    virtual inline T* Data();

    /// <summary>
    /// </summary>
    /// <returns>How many bytes long the array is.</returns>
    inline u64 SizeInBytes() const;

    /// <summary>
    /// </summary>
    /// <returns>How many bytes long the memory is.</returns>
    inline u64 BufferSize() const;
    
    /// <summary>
    /// </summary>
    /// <returns>How many elements long the array is.</returns>
    inline int Count() const;
    
    /// <summary>
    /// </summary>
    /// <returns>The allocator of the array.</returns>
    inline Allocator Allocator() const;
    
    /// <summary>
    /// Returns a slice of <c>count</c> elements starting at <c>offset</c>.
    /// </summary>
    /// <param name="offset">The offset.</param>
    /// <param name="count">The count. -1 for remaining elements.</param>
    /// <returns>The slice.</returns>
    inline Slice<T> Slice(int offset = 0, int count = -1);

    /// <summary>
    /// Returns a reinterpreted slice of <c>count</c> elements starting at <c>offset</c>.
    /// </summary>
    /// <param name="offset">The offset.</param>
    /// <param name="count">The count. -1 for remaining elements.</param>
    /// <returns>The reinterpreted slice.</returns>
    template<typename U>
    inline pd::Slice<U> SliceAs(int offset = 0, int count = -1);

    T& operator[](int i);

    // Delete memory and Copy by reference
    void operator=(Array<T>& other);

protected:
    /// <summary>
    /// Grows the memory by to accomodate <c>newCount</c> elements.
    /// </summary>
    /// <param name="newCount">The new count.</param>
    /// <param name="construct">If <c>true</c> it will call the constructor on each new element.</param>
    void Grow(int newCount, bool construct = true);
    
    /// <summary>
    /// Constructs <c>count</c> elements starting from <c>index</c>
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="count">The count.</param>
    void Construct(int index, int count);
    
    /// <summary>
    /// Destructs <c>count</c> elements starting from <c>index</c>
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="count">The count.</param>
    void Destruct(int index, int count);

    pd::Allocator allocator = Allocator::None;
    T* memory = nullptr;
    u64 bufferSize = 0;
    int count = 0;
};

template<typename T, int maxCapacity>
class BoundedArray final : public Array<T> {
public:
    BoundedArray() : Array(Allocator::None) {
        bufferSize = maxCapacity * sizeof(T);
    }

    virtual inline T* Data() {
        memory = (T*)&stackMemory[0];
        return memory;
    }

    virtual void ChangeAllocator(pd::Allocator allocator) {
        // Not allowed
    }

private:
    // The reason this is a byte array and not an array of `T` is because we don't to automatically construct or destruct any elements
    byte stackMemory[maxCapacity * sizeof(T)] = {};
};

// Print

template<typename U>
inline void PrintType(Slice<U>* type, FormatInfo* info) {
    PrintfToStream(info->output, "{%s", (info->pretty) ? "\n    " : " ");

    int count = type->Count();

    if (info->precisionSpecified && info->precision >= 0 && info->precision < count) {
        count = info->precision;
    }

    for (int i = 0; i < count; i++) {
        PrintType((U*)&type->At(i), info);

        if (i + 1 < count) {
            PrintfToStream(info->output, ",%s", (info->pretty) ? "\n    " : " ");
        }
    }

    PrintfToStream(info->output, "%s}", (info->pretty) ? "\n" : " ");
}

template<typename U>
inline void PrintType(Array<U>* type, FormatInfo* info) {
    Slice<U> slice = type->Slice();
    PrintType(&slice, info);
}

//
// Implementation
//

template<typename T>
inline Array<T>::Array(pd::Allocator allocator) : allocator(allocator) {}

template<typename T>
inline Array<T>::~Array() {
    Delete();
}

template<typename T>
inline void Array<T>::Delete() {
    Destruct(0, count);

    Free(memory, allocator);

    memory = nullptr;
    count = 0;
    bufferSize = 0;
}

template<typename T>
inline void Array<T>::Resize(int newCount) {
    if (newCount > count) {
        Grow(newCount, false);
    } else if (newCount < count) {
        // Shrink and destruct any elements necessary
        Destruct(newCount, count - newCount);
        count = newCount;
    }
}

template<typename T>
inline int Array<T>::Add(const T& t) {
    return Add(&t);
}

template<typename T>
inline int Array<T>::Add(const T* t) {
    Grow(count + 1);
    memory[count] = *((T*)t);

    return count++;
}

template<typename T>
template<typename ...Args>
inline int Array<T>::Add(Args ...args) {
    Grow(count + 1);
    new (&memory[count]) T(args...);

    return count++;
}

template<typename T>
template<typename U>
inline pd::Slice<U> Array<T>::SliceAs(int offset, int count) {
    return Slice(offset, count).As<U>();
}

template<typename T>
inline int Array<T>::AddRange(T* memory, int count) {
    int startIndex = this->count;

    Grow(startIndex + count);
    for (int i = 0; i < count; i++) {
        this->memory[startIndex + i] = memory[i];
    }

    this->count += count;

    return startIndex;
}

template<typename T>
inline int Array<T>::AddRange(const pd::Slice<T>& elements) {
    return AddRange(elements.Data(), elements.Count());
}

template<typename T>
inline void Array<T>::Insert(int index, const T& t) {
    Insert(index, &t);
}

template<typename T>
inline void Array<T>::Insert(int index, const T* t) {
    PD_ASSERT_D(index >= 0 && index < Count(),
                "illegal insertion index, valid: 0:%d, given: %d", Count(), index);

    if (index == count) {
        // If we're trying to insert at the end we can just Add it
        Add(t);
    } else {
        Resize(count + 1);
        // Move everything to the right
        MemoryCopy(Data() + index + 1,
                   Data() + index,
                   (count++ - index) * sizeof(T), true);
        Data()[index] = *t;
    }
}

template<typename T>
inline void Array<T>::InsertUnordered(int index, const T& t) {
    InsertUnordered(index, &t);
}

template<typename T>
inline void Array<T>::InsertUnordered(int index, const T* t) {
    PD_ASSERT_D(index >= 0 && index < Count(),
                "valid insertion index, valid: 0:%d, given: %d", Count(), index);

    int addedIndex = Add(t);

    // If we didn't Add it to the desired location, swap it
    if (index != addedIndex) {
        Swap(index, addedIndex);
    }
}

template<typename T>
inline void Array<T>::Swap(int a, int b) {
    if (a == b) return;

    PD_ASSERT_D(a >= 0 && a < Count() && b >= 0 && b < Count(),
                "illegal swap indices, valid: 0:%d, given %d,%d", Count(), a, b);

    T tmp = Data()[a];
    Data()[a] = Data()[b];
    Data()[b] = tmp;
}

template<typename T>
inline void Array<T>::Reserve(int reservationCount) {
    PD_ASSERT_D(reservationCount > 0, "reservation count must be positive, given: %d", reservationCount);

    Resize(count + reservationCount);
    Construct(count, reservationCount);

    count += reservationCount;
}

template<typename T>
inline void Array<T>::Remove(int index) {
    PD_ASSERT_D(index >= 0 && index < Count(),
                "illegal removal index, valid: 0:%d, given: %d", Count(), index);

    Destruct(index, 1);

    if (index == count - 1) {
        // If it's the last element we can just decrease the count
        count -= 1;
    } else {
        MemoryCopy(Data() + index,
                   Data() + index + 1,
                   ((u64)count - (u64)index) * sizeof(T));

        count -= 1;
    }
}

template<typename T>
inline void Array<T>::RemoveUnordered(int index) {
    PD_ASSERT_D(index >= 0 && index < Count(),
                "illegal removal index, valid: 0:%d, given: %d", Count(), index);

    Destruct(index, 1);

    count -= 1;
    if (index != count) {
        // Put the last element in it's place
        At(index) = At(count);
    }
}

template<typename T>
inline void Array<T>::RemoveRange(int index, int count) {
    if (count <= 0) {
        count = this->count - index;
    }

    PD_ASSERT_D(index >= 0 && index < Count() && index + count <= Count(),
                "illegal removal range, valid: 0:%d, given: %d:%d", Count(), index, count);

    Destruct(index, count);

    this->count -= count;
    MemoryCopy(Data() + index,
               Data() + index + count,
               (this->count - index) * sizeof(T), true);
}

template<typename T>
inline void Array<T>::Clear() {
    Destruct(0, count);
    count = 0;
}

template<typename T>
template<typename LessLambda>
inline void Array<T>::Sort(LessLambda func, int offset, int count) {
    if (Count() == 0 || (count >= 0 && offset >= (offset + count - 1))) return;

    if (count <= 0) {
        count = this->count - offset;
    }

    PD_ASSERT_D(offset >= 0 && offset < Count() && offset + count <= Count(),
                "illegal sorting range, valid: 0:%d, given: %d:%d", Count(), offset, count);

    // Quick sort baby!

    const auto partition = [func, this](T* memory, int start, int end) {
        int i = start;
        for (int j = start; j < end; j++) {
            if (func(memory[j], memory[end])) {
                Swap(i, j);
                i += 1;
            }
        }

        Swap(i, end);
        return i;
    };

    // Recursive lambda, see https://stackoverflow.com/a/40873505
    // The `-> void` is required on `quickSortImpl` or else it thinks is fooled
    // into believing it returns auto and it throws a big fat error
    const auto quickSort = [partition](T* memory, int start, int end) {
        auto quickSortImpl = [partition](T* memory, int start, int end, auto& quick) -> void {
            if (start < end) {
                int p = partition(memory, start, end);
                quick(memory, start, p - 1, quick);
                quick(memory, p + 1, end, quick);
            }
        };
        quickSortImpl(memory, start, end, quickSortImpl);
    };

    quickSort(Data(), offset, offset + count - 1);
}

template<typename T>
template<typename FindLabmda>
inline int Array<T>::Find(FindLabmda func, int offset, int count) {
    if (Count() == 0) return -1;

    if (count <= -1) {
        count = this->count - offset;
    }

    PD_ASSERT_D(offset >= 0 && offset < Count() && offset + count <= Count(),
                "illegal find range, valid: 0:%d, given: %d:%d", Count(), offset, count);

    for (int i = offset; i < count; ++i) {
        if (func(Data()[i])) {
            return i;
        }
    }

    return -1;
}

template<typename T>
inline void Array<T>::ChangeAllocator(pd::Allocator allocator) {
    if (this->allocator == allocator) return;

    if (allocator == pd::Allocator::None) {
        Delete();
    } else if (allocator == pd::Allocator::Persistent) {
        T* newMemory = (T*)Alloc(bufferSize, allocator);
        MemoryCopy(newMemory, memory, bufferSize);

        // We know that `memory` doesn't use the persistent allocator so we don't need to free it
        memory = newMemory;
    } else if (allocator == pd::Allocator::Temporary) {
        T* newMemory = (T*)Alloc(bufferSize, allocator);
        MemoryCopy(newMemory, memory, bufferSize);

        if (this->allocator == pd::Allocator::Persistent) {
            Free(memory);
        }

        memory = newMemory;
    }

    this->allocator = allocator;
}

template<typename T>
inline T& Array<T>::At(int index) {
    return Data()[index];
}

template<typename T>
inline T& Array<T>::First() {
    return At(0);
}

template<typename T>
inline T& Array<T>::Last() {
    return At(count - 1);
}

template<typename T>
inline T* Array<T>::Data() {
    return memory;
}

template<typename T>
inline u64 Array<T>::SizeInBytes() const {
    return count * sizeof(T);
}

template<typename T>
inline u64 Array<T>::BufferSize() const {
    return bufferSize;
}

template<typename T>
inline int Array<T>::Count() const {
    return count;
}

template<typename T>
inline Allocator Array<T>::Allocator() const {
    return allocator;
}

template<typename T>
inline Slice<T> Array<T>::Slice(int offset, int count) {
    if (count < 0) {
        count = Count() - offset;
    }

    return pd::Slice<T>(Data() + offset, count);
}

template<typename T>
inline T& Array<T>::operator[](int i) {
    return At(i);
}

template<typename T>
inline void Array<T>::operator=(Array<T>& other) {
    Delete();
    this->memory = other.Data();
    this->bufferSize = other.SizeInBytes();
    this->count = other.Count() + 1;
    this->allocator = other.Allocator();
}

template<typename T>
inline void Array<T>::Grow(int newCount, bool construct) {
    // Should these be in here?
    const int INITIAL_LENGTH = 32;
    const int GROW_FACTOR = 2;

    if (newCount > bufferSize / sizeof(T)) {
        if (!Data()) {
            bufferSize = INITIAL_LENGTH * sizeof(T);
        }

        while (bufferSize < newCount * sizeof(T)) {
            bufferSize *= GROW_FACTOR;
        }

        memory = (T*)Realloc(Data(), bufferSize, allocator);
    }

    // Construct new memory
    if (construct && newCount > count) {
        Construct(count, newCount - count);
    }
}

template<typename T>
inline void Array<T>::Construct(int index, int count) {
    if (count == 1) {
        new (Data() + index) T();
    } else {
        for (int i = 0; i < count; i++) {
            new (Data() + index + i) T();
        }
    }
}

template<typename T>
inline void Array<T>::Destruct(int index, int count) {
    if (count == 1) {
        Data()[index].~T();
    } else {
        for (int i = 0; i < count; i++) {
            Data()[i].~T();
        }
    }
}

}
