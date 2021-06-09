#pragma once

#include <new> // emplace new
#include <utility> // std::forward

#include "Pandora/Core/Types.h"

namespace pd {

/**
 * \brief `Persistent` allocates from the persistent allocation pool.
 * `Temporary` allocates from a ring buffer. Does not need to be freed.
 * `None` does nothing.
 */
enum class Allocator : byte {
    None,
    Persistent,
    Temporary
};

/**
 * \param size The allocation size in bytes.
 * \param type The allocator to use.
 * \return The allocated memory. Will return a nullptr if allocation fails.
 */
void* Alloc(u64 size, Allocator type = Allocator::Persistent);

/**
 * \brief Reallocates an existing buffer.
 * Buffer must be allocated
 * 
 * \param ptr The previously allocated buffer.
 * When nullptr is passed it will `Alloc()` an new buffer.
 * \param size The new size in bytes.
 * \param type The allocator to use. Must match the previously used allocator.
 * \return The reallocated buffer. Will return nullptr if reallocation fails.
 */
void* Realloc(void* ptr, u64 size, Allocator type = Allocator::Persistent);

/**
 * \brief Frees the buffer.
 * 
 * \param ptr The previosly allocated buffer.
 * \param type The allocator to use. Must match the previously used allocator.
 */
void Free(void* ptr, Allocator type = Allocator::Persistent);

/**
 * \brief Frees all the memory associated with the temporary allocator.
 * Call this at the end of your program.
 */
void DeleteTemporaryAllocator();

/**
 * \return How many bytes haven't been freed yet.
 */
u64 GetAllocatedBytes();

/**
 * \param ptr The previously allocated buffer.
 * \param type The allocator to use.
 * \return How big the allocated buffer is.
 */
u64 GetAllocatedSize(void* ptr, Allocator type = Allocator::Persistent);

/**
 * \param ptr The previously allocated buffer.
 * \return The allocator used for the allocation.
 */
Allocator GetAllocator(void* ptr);

/**
 * \brief Allocates and constructs a new `T` with the persistent allocator.
 * 
 * \tparam T The type to allocate and construct.
 * \tparam Args The constructor arguments.
 * \param args The constructor arguments.
 * \return The allocated object.
 */
template<typename T, typename ...Args>
T* New(Args&&... args) {
    T* t = (T*)Alloc(sizeof(T));
    new (t) T(std::forward<Args>(args)...);

    return t;
}

/**
 * \brief Destructs and frees a previously constructed object.
 * 
 * \tparam T The type to destruct and free.
 * \param t The object.
 */
template<typename T>
void Delete(T* t) {
    t->~T();
    Free(t);
}

}
