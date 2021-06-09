#include "Memory.h"

#include <memory.h>

#include "Pandora/Core/Math/Math.h"
#include "Pandora/Core/Data/Allocator.h"

namespace pd {

inline void MemoryCopy(const void* destination, u64 destinationSize, const void* source, u64 copySize, bool useIntermediateBuffer) {
    MemoryCopy(destination, source, Min(destinationSize, copySize), useIntermediateBuffer);
}

void MemoryCopy(const void* destination, const void* source, u64 copySize, bool useIntermediateBuffer) {
    const void* intermediate = (useIntermediateBuffer) ? Alloc(copySize) : destination;

    // Copy 8 bytes at a time
    for (int i = 0; i < copySize / sizeof(u64); i++) {
        ((u64*)intermediate)[i] = ((u64*)source)[i];
    }

    // Copy any remaining bytes
    int base = (int)(copySize / sizeof(u64) * sizeof(u64));
    for (int i = 0; i < copySize % sizeof(u64); i++) {
        ((byte*)intermediate)[base + i] = ((byte*)source)[base + i];
    }

    if (useIntermediateBuffer) {
        // Copy 8 bytes at a time
        for (int i = 0; i < copySize / sizeof(u64); i++) {
            ((u64*)destination)[i] = ((u64*)intermediate)[i];
        }

        // Copy any remaining bytes
        for (int i = 0; i < copySize % sizeof(u64); i++) {
            ((byte*)destination)[base + i] = ((byte*)intermediate)[base + i];
        }

        Free((void*)intermediate);
    }
}

void MemorySet(void* destination, u64 destinationSize, u64 setSize, byte value) {
    MemorySet(destination, Min(destinationSize, setSize), value);
}

void MemorySet(void* destination, u64 setSize, byte value) {
    u64 v = (u64)value;
    u64 packedValue = (v << 56) | (v << 48) | (v << 40) | (v << 32) | (v << 24) | (v << 16) | (v << 8) | v;

    // Copy 8 bytes at a time
    for (int i = 0; i < setSize / sizeof(u64); i++) {
        ((u64*)destination)[i] = packedValue;
    }

    // Copy any remaining bytes
    int base = (int)(setSize / sizeof(u64) * sizeof(u64));
    for (int i = 0; i < setSize % sizeof(u64); i++) {
        ((byte*)destination)[base + i] = value;
    }

}

bool MemoryCompare(void* a, void* b, u64 compareSize) {
    // Compare 8 bytes at a time
    for (int i = 0; i < compareSize / sizeof(u64); i++) {
        if (((u64*)a)[i] != ((u64*)b)[i]) {
            return false;
        }
    }

    // Compare any remaining bytes
    int base = (int)(compareSize / sizeof(u64) * sizeof(u64));
    for (int i = 0; i < compareSize % sizeof(u64); i++) {
        if (((byte*)a)[base + i] != ((byte*)b)[base + i]) {
            return false;
        }
    }

    return true;
}

}
