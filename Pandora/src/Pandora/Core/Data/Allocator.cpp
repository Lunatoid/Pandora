#include "Allocator.h"

#include <malloc.h>
#include <memory>

#include "Pandora/Core/Assert.h"
#include "Pandora/Core/Math/Math.h"
#include "Pandora/Core/Data/Memory.h"

// Disable "Dereferencing NULL pointer"
#pragma warning(push)
#pragma warning(disable: 6011)

namespace pd {

//
// Persistent allocator
//

// @GLOBAL
static u64 persistentAllocated = 0;

struct AllocationHeader {
    Allocator type;
    u64 size;
};

inline void* PersistentAlloc(u64 size) {
    AllocationHeader* ptr = (AllocationHeader*)malloc(size + sizeof(AllocationHeader));
    PD_ASSERT_D(ptr != nullptr, "failed to allocate %lld bytes with the persistent allocator", size);


    ptr->type = Allocator::Persistent;
    ptr->size = size;

    persistentAllocated += size;
    return ptr + 1;
}

inline void* PersistentRealloc(void* ptr, u64 size) {
    if (ptr) {
        u64 prevSize = ((u64*)ptr)[-1];
        bool sub = size < prevSize;
        if (sub) {
            persistentAllocated -= prevSize - size;
        } else {
            persistentAllocated += size - prevSize;
        }
    } else {
        persistentAllocated += size;
    }

    byte* alignedPtr = (byte*)ptr;
    if (ptr) {
        alignedPtr -= sizeof(AllocationHeader);
    }

    AllocationHeader* newPtr = (AllocationHeader*)realloc(alignedPtr, size + sizeof(AllocationHeader));
    PD_ASSERT_D(newPtr != nullptr, "failed to reallocate %lld bytes with the persistent allocator", size);

    newPtr->size = size;

    return newPtr + 1;
}

inline void PersistentFree(void* ptr) {
    byte* alignedPtr = (byte*)ptr;

    if (ptr) {
        AllocationHeader* header = &((AllocationHeader*)ptr)[-1];
        persistentAllocated -= header->size;

        alignedPtr -= sizeof(AllocationHeader);
    }

    free(alignedPtr);
}

//
// Temporary allocator
//

// @GLOBAL
struct {
    byte* memory = nullptr;
    u64 bufferSize = 0;
    u64 usedBytes = 0;
} tempAllocData;

inline void InitTemporaryMemory() {
    if (!tempAllocData.memory) {
        // @TODO: make this controllable
        tempAllocData.bufferSize = (u64)Megabytes(16);
        tempAllocData.memory = (byte*)malloc(tempAllocData.bufferSize); // 16MB
    }
}

inline void* TemporaryAlloc(u64 size) {
    InitTemporaryMemory();

    size += sizeof(AllocationHeader);

    // Check if we can fit the allocation, if not we flow back to the Start of the memory
    if (tempAllocData.usedBytes + size > tempAllocData.bufferSize) {
        tempAllocData.usedBytes = 0;
    }

    byte* memory = tempAllocData.memory + tempAllocData.usedBytes;

    // Encode allocated bufferSize
    if (!memory) return nullptr;

    ((AllocationHeader*)memory)->type = Allocator::Temporary;
    ((AllocationHeader*)memory)->size = size - sizeof(AllocationHeader);

    tempAllocData.usedBytes += size;

    return memory + sizeof(AllocationHeader);
}

inline void* TemporaryRealloc(void* ptr, u64 size) {
    InitTemporaryMemory();

    // Replicate malloc/realloc behaviour of allocating on receiving a nullptr
    if (!ptr) {
        return TemporaryAlloc(size);
    }

    byte* memory = (byte*)ptr - sizeof(AllocationHeader);
    u64 oldSize = ((AllocationHeader*)memory)->size;

    // @TODO: optimize this where it checks if we're the last allocation made and if so we just grab remaining space if possible.
    // This saves us having to allocate and copy
    byte* newPtr = (byte*)TemporaryAlloc(size);
    MemoryCopy(newPtr, ptr, Min(size, oldSize), true);

    return newPtr;
}

//
// Allocator API
//

void* Alloc(u64 size, Allocator type) {
    if (type == Allocator::Persistent) {
        return PersistentAlloc(size);
    } else if (type == Allocator::Temporary) {
        return TemporaryAlloc(size);
    }

    return nullptr;
}

void* Realloc(void* ptr, u64 size, Allocator type) {
    if (type == Allocator::Persistent) {
        return PersistentRealloc(ptr, size);
    } else if (type == Allocator::Temporary) {
        return TemporaryRealloc(ptr, size);
    }

    return nullptr;
}

void Free(void* ptr, Allocator type) {
    if (type == Allocator::Persistent) {
        PersistentFree(ptr);
    }
}

void DeleteTemporaryAllocator() {
    if (tempAllocData.memory) {
        free(tempAllocData.memory);
        tempAllocData.bufferSize;
        tempAllocData.usedBytes = 0;
    }
}

u64 GetAllocatedBytes() {
    return persistentAllocated;
}

u64 GetAllocatedSize(void* ptr, Allocator type) {
    if (type == Allocator::Persistent || type == Allocator::Temporary) {
        return ((AllocationHeader*)ptr)[-1].size;
    }

    return 0;
}

Allocator GetAllocator(void* ptr) {
    return ((AllocationHeader*)ptr)[-1].type;
}

}

#pragma warning(pop)
