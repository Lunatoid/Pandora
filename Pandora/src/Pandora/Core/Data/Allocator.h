#pragma once

#include <new>

#include "Pandora/Core/Types.h"

namespace pd {

/// <summary>
/// <c>Persistent</c>: wrapper for <c>malloc()</c>.
/// <c>Temporary</c>: will allocate from a ring memory.
/// <c>None</c>: do nothing.
/// </summary>
enum class Allocator : byte {
    None,
    Persistent,
    Temporary
};

/// <summary>
/// Allocates a memory of <c>size</c> bytes with the specified allocator.
/// </summary>
/// <param name="size">The desired size in bytes.</param>
/// <param name="type">The allocator to use.</param>
/// <returns>The allocated memory.</returns>
void* Alloc(u64 size, Allocator type = Allocator::Persistent);

/// <summary>
/// Reallocates a memory of <c>size</c> bytes with the specified allocator.
/// </summary>
/// <param name="ptr">A pointer to the old memory, allocated with the same allocator.</param>
/// <param name="size">The desired size in bytes.</param>
/// <param name="type">The allocator to use.</param>
/// <returns>The reallocated memory.</returns>
void* Realloc(void* ptr, u64 size, Allocator type = Allocator::Persistent);

/// <summary>
/// Frees the memory.
/// </summary>
/// <param name="ptr">The memory.</param>
void Free(void* ptr, Allocator type = Allocator::Persistent);

/// <summary>
/// Frees all memory related to the temporary allocator.
/// </summary>
void DeleteTemporaryAllocator();

/// <summary>
/// Returns the allocated bytes that haven't been freed yet.
/// </summary>
/// <returns>The allocated bytes that haven't been freed yet.</returns>
u64 GetAllocatedBytes();

/// <summary>
/// Returns the requested size of the allocated pointer.
/// </summary>
/// <param name="ptr">The memory.</param>
/// <param name="type">The allocator used to allocate <c>ptr</c></param>
/// <returns>The requested size in bytes.</returns>
u64 GetAllocatedSize(void* ptr, Allocator type = Allocator::Persistent);

/// <summary>
/// Returns the allocator used to allocate <c>ptr</c>.
/// </summary>
/// <param name="ptr">The memory.</param>
/// <returns>The allocator used to allocate <c>ptr</c>.</returns>
Allocator GetAllocator(void* ptr);

/// <summary>
/// Allocates and constructs <c>T</c> with the persitent allocator.
/// </summary>
/// <param name="...args">The constructor args.</param>
/// <returns>The allocated <c>T</c>.</returns>
template<typename T, typename ...Args>
T* New(Args&&... args) {
    T* t = (T*)Alloc(sizeof(T));
    new (t) T(std::forward<Args>(args)...);

    return t;
}

/// <summary>
/// Destructs and frees the <c>t</c> that was allocated with <c>New()</c>.
/// </summary>
/// <param name="t">The <c>t</c>.</param>
template<typename T>
void Delete(T* t) {
    t->~T();
    Free(t);
}

}
