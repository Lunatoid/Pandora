#pragma once

#include <memory.h>
#include <new>

#include "Pandora/Core/Assert.h"
#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/Allocator.h"
#include "Pandora/Core/Data/Memory.h"
#include "Pandora/Core/Data/Slice.h"
#include "Pandora/Core/Data/Optional.h"
#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

template<typename T>
class Array {
public:

    /**
     * \param allocator The allocator to use.
     */
    Array(Allocator allocator = Allocator::Persistent);

    virtual ~Array();

    /**
     * \brief Destructs all elements and frees the buffer 
     * Gets called on destruction.
     */
    void Delete();

    /**
     * \brief Resizes the buffer to hold `newCount` elements.
     * 
     * \param newCount The new buffer size.
     */
    void Resize(int newCount);

    /**
     * \brief Adds a new elements at the end of the array.
     * 
     * \param t The new element.
     * \return The index of the new element.
     */
    int Add(const T& t);

    /**
     * \brief Adds a new elements to the end of the array.
     * 
     * \param t A pointer to the new element.
     * \return The index of the new element.
     */
    int Add(const T* t);

    /**
     * \brief Constructs a new element at the end of the array.
     * 
     * \param args The constructor arguments.
     * \return The index of the newly constructed element.
     */
    template<typename ...Args>
    int Add(Args ...args);

    /**
     * \brief Adds a range of elements to the end of the array.
     * 
     * \param memory The buffer.
     * \param count The buffer size.
     * \return The starting index of the newly added elements.
     */
    int AddRange(T* memory, int count);

    /**
     * \brief Adds a range of elements to the end of the array.
     * 
     * \param elements A slice with all the elements to add.
     * \return The starting index of the newly added elements.
     */
    int AddRange(const Slice<T>& elements);

    /**
     * \brief Inserts an element into the buffer at the specified index.
     * This operation is ordered.
     * 
     * \param index The index.
     * \param t The new element.
     */
    void Insert(int index, const T& t);

    /**
     * \brief Inserts an element into the buffer at the specified index.
     * This operation is ordered.
     * 
     * \param index The index.
     * \param t The element.
     */
    void Insert(int index, const T* t);

    /**
     * \brief Inserts an element into the buffer at the specified index.
     * This operation is unordered.
     * 
     * \param index The index.
     * \param t The element.
     */
    void InsertUnordered(int index, const T& t);

    /**
     * \brief Inserts an element into the buffer at the specified index.
     * This operation is unordered.
     * 
     * \param index The index.
     * \param t The element.
     */
    void InsertUnordered(int index, const T* t);

    /**
     * \brief Swaps two elements.
     * 
     * \param a The first element to swap.
     * \param b The second element to swap.
     */
    void Swap(int a, int b);

    /**
     * \brief Adds `reservationCount` default-constructed elements.
     * 
     * \param reservationCount How many elements to reserve.
     */
    void Reserve(int reservationCount);

    /**
     * \brief Removes the element at the specified index.
     * This operation is ordered.
     * 
     * \param index The index.
     */
    void Remove(int index);

    /**
     * \brief Removes the element at the specified index.
     * This operation is unordered.
     * 
     * \param index The index.
     */
    void RemoveUnordered(int index);

    /**
     * \brief Removes a range of elements.
     * This operation is ordered.
     * 
     * \param index The starting index.
     * \param count How many elements to remove from the starting index.
     */
    void RemoveRange(int index, int count);

    /**
     * \brief Destructs all the elements and resets the count.
     * Does not free the buffer.
     */
    inline void Clear();

    /**
     * \brief Sorts the buffer using quicksort.
     * 
     * \tparam LessLambda The sort function. Generic lambdas are possible.
     * \param func The sort function. Needs to return false if `a` is less than `b`.
     * \param offset The offset from the start.
     * \param count How many elements to sort. Pass -1 for all remaining elements.
     */
    template<typename LessLambda>
    void Sort(LessLambda func, int offset = 0, int count = -1);

    /**
     * \brief Finds a specific element in the buffer.
     * 
     * \tparam FindLambda The find function. Generic lambdas are possible.
     * \param func The find function. Needs to return true if the element is a match.
     * \param offset The offset from the start.
     * \param count How many elements to reach through. Pass -1 for all remaining elements.
     * \return The index, if found.
     */
    template<typename FindLambda>
    Optional<int> Find(FindLambda func, int offset = 0, int count = -1);

    /**
     * \brief Changes the allocator.
     * If `Allocator::None` is passed, `Delete()` is called.
     * If a different allocator is passed, all memory is copied to a new buffer
     * and the old buffer is freed.
     * 
     * \param allocator The new allocator.
     */
    virtual void ChangeAllocator(pd::Allocator allocator);

    /**
     * \param index The index.
     * \return The element at the specified index.
     */
    inline T& At(int index) const;

    /**
     * \return The first element.
     */
    inline T& First();

    /**
     * \return The last element.
     */
    inline T& Last();

    /**
     * \return The raw data pointer of the buffer.
     */
    virtual inline T* Data() const;

    /**
     * \return The element count in bytes.
     */
    inline u64 SizeInBytes() const;

    /**
     * \return How buffer size in bytes.
     */
    inline u64 BufferSize() const;

    /**
     * \return How many elements are in the array.
     */
    inline int Count() const;

    /**
     * \return The allocator used for the buffer.
     */
    inline Allocator Allocator() const;

    /**
     * \brief Creates a slice based on the array.
     * 
     * \param offset The offset from the start.
     * \param count How many elements should be in the slice. Pass -1 for all remaining elements.
     * \return The slice.
     */
    inline pd::Slice<T> Slice(int offset = 0, int count = -1);

    /**
     * \brief Creates a slice and casts it to a new type.
     * 
     * \tparam U The type to cast it at.
     * \param offset The offset from the start.
     * \param count How many elements should be in the slice. Pass -1 for all remaining elements.
     * \return The slice.
     */
    template<typename U>
    inline pd::Slice<U> SliceAs(int offset = 0, int count = -1);

    T& operator[](int i) const;

    // Delete memory and Copy by reference
    void operator=(const Array<T>& other);

    template<typename U>
    struct ArrayIt {
        ArrayIt(const Array<U>& parent, int i)
            : parent(parent), i(i) {}

        T& operator*() const {
            return parent.At(i);
        }

        void operator++() {
            i += 1;
        }

        bool operator==(const ArrayIt<U>& other) const {
            return i == other.i && parent.Data() == other.parent.Data();
        }

        bool operator!=(const ArrayIt<U>& other) const {
            return !operator==(other);
        }

    private:
        int i = 0;
        const Array<U>& parent;
    };

    // Ranged for begin/end functions
    inline ArrayIt<T> begin() const;
    inline ArrayIt<T> end() const;

protected:

    /**
     * \brief Grows the buffer to accomodate `newCount` elements.
     * 
     * \param newCount The new count.
     * \param construct Whether or not to construct the new elements.
     */
    void Grow(int newCount, bool construct = true);

    /**
     * \brief Constructs elements in the given range.
     * 
     * \param index The starting index.
     * \param count The count from the index.
     */
    void Construct(int index, int count);

    /**
     * \brief Destructs elements in the given range.
     * 
     * \param index The starting index.s
     * \param count The count from the index.
     */
    void Destruct(int index, int count);

    pd::Allocator allocator = Allocator::None;
    T* memory = nullptr;
    u64 bufferSize = 0;
    int count = 0;
};

template<typename T, int maxCapacity>
class BoundedArray final : public Array<T> {
public:
    BoundedArray() : Array<T>(Allocator::None) {
        this->bufferSize = maxCapacity * sizeof(T);
    }

    virtual ~BoundedArray() {
        // For some reason when ~Array() calls Data() it calls it's own version
        // and not the overloaded version
        this->memory = Data();
    }

    virtual inline T* Data() const {
        return (T*)&stackMemory[0];
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
inline void PrintType(Slice<U>& type, FormatInfo& info) {
    PrintfToStream(info.output, "{%s", (info.pretty) ? "\n    " : " ");

    int count = type.Count();

    if (info.precisionSpecified && info.precision >= 0 && info.precision < count) {
        count = info.precision;
    }

    for (int i = 0; i < count; i++) {
        PrintType((U&)type.At(i), info);

        if (i + 1 < count) {
            PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
        }
    }

    PrintfToStream(info.output, "%s}", (info.pretty) ? "\n" : " ");
}

template<typename U>
inline void PrintType(Array<U>& type, FormatInfo& info) {
    Slice<U> slice = type.Slice();
    PrintType(slice, info);
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
    Destruct(0, Count());

    if (memory) {
        Free(memory, allocator);
        memory = nullptr;
    }

    count = 0;
    bufferSize = 0;
}

template<typename T>
inline void Array<T>::Resize(int newCount) {
    if (newCount > Count()) {
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
    Data()[count] = *((T*)t);

    return count++;
}

template<typename T>
template<typename ...Args>
inline int Array<T>::Add(Args ...args) {
    Grow(count + 1);
    new (&Data()[count]) T(std::forward<Args>(args)...);

    return count++;
}

template<typename T>
template<typename U>
inline pd::Slice<U> Array<T>::SliceAs(int offset, int count) {
    return Slice(offset, count).template As<U>();
}

template<typename T>
inline int Array<T>::AddRange(T* memory, int count) {
    int startIndex = this->count;

    Grow(startIndex + count);
    for (int i = 0; i < count; i++) {
        Data()[startIndex + i] = memory[i];
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
    PD_ASSERT_D(reservationCount >= 0, "reservation count must be positive, given: %d", reservationCount);

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

    count -= 1;
    if (index != count) {
        // Put the last element in it's place
        At(index) = At(count);
    }
    Destruct(count, 1);
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
inline Optional<int> Array<T>::Find(FindLabmda func, int offset, int count) {
    if (Count() == 0) return Optional<int>();

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

    return Optional<int>();
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
inline T& Array<T>::At(int index) const {
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
inline T* Array<T>::Data() const {
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
inline typename Array<T>::template ArrayIt<T> Array<T>::begin() const {
    return ArrayIt<T>(*this, 0);
}

template<typename T>
inline typename Array<T>::template ArrayIt<T> Array<T>::end() const {
    return ArrayIt<T>(*this, Count());
}

template<typename T>
inline T& Array<T>::operator[](int i) const {
    return At(i);
}

template<typename T>
inline void Array<T>::operator=(const Array<T>& other) {
    Delete();

    Reserve(other.Count());

    for (int i = 0; i < other.Count(); i++) {
        Data()[i] = other.Data()[i];
    }
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
        At(index).~T();
    } else {
        for (int i = index; i < count; i++) {
            At(i).~T();
        }
    }
}

}
