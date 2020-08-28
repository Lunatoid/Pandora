#include "Atomics.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

namespace pd {

i64 AtomicInc(i64* dest) {
#if defined(PD_WINDOWS)
    return InterlockedIncrement64((volatile LONG64*)dest);
#else
#error Implement me!
#endif
}

i32 AtomicInc(i32* dest) {
#if defined(PD_WINDOWS)
    return InterlockedIncrement((volatile LONG*)dest);
#else
#error Implement me!
#endif
}

i16 AtomicInc(i16* dest) {
#if defined(PD_WINDOWS)
    return InterlockedIncrement16((volatile SHORT*)dest);
#else
#error Implement me!
#endif
}

i64 AtomicDec(volatile i64* dest) {
#if defined(PD_WINDOWS)
    return InterlockedDecrement64((volatile LONG64*)dest);
#else
#error Implement me!
#endif
}

i32 AtomicDec(volatile i32* dest) {
#if defined(PD_WINDOWS)
    return InterlockedDecrement((volatile LONG*)dest);
#else
#error Implement me!
#endif
}

i16 AtomicDec(volatile i16* dest) {
#if defined(PD_WINDOWS)
    return InterlockedDecrement16((volatile SHORT*)dest);
#else
#error Implement me!
#endif
}

i32 AtomicAdd(volatile i32* dest, i32 val) {
#if defined(PD_WINDOWS)
    return InterlockedAdd((volatile LONG*)dest, val);
#else
#error Implement me!
#endif
}

i64 AtomicAdd(volatile i64* dest, i64 val) {
#if defined(PD_WINDOWS)
    return InterlockedAdd64((volatile LONG64*)dest, val);
#else
#error Implement me!
#endif
}

i32 AtomicSub(volatile i32* dest, i32 val) {
    return AtomicAdd(dest, -val);
}

i64 AtomicSub(volatile i64* dest, i64 val) {
    return AtomicAdd(dest, -val);
}

i8 AtomicStore(volatile i8* dest, i8 val) {
#if defined(PD_WINDOWS)
    return InterlockedExchange8((volatile CHAR*)dest, val);
#else
#error Implement me!
#endif
}

bool AtomicStore(volatile bool* dest, bool val) {
    return AtomicStore((volatile i8*)dest, (i8)val) == 1;
}

i16 AtomicStore(volatile i16* dest, i16 val) {
#if defined(PD_WINDOWS)
    return InterlockedExchange16((volatile SHORT*)dest, val);
#else
#error Implement me!
#endif
}

i32 AtomicStore(volatile i32* dest, i32 val) {
#if defined(PD_WINDOWS)
    return InterlockedExchange((volatile LONG*)dest, val);
#else
#error Implement me!
#endif
}

i64 AtomicStore(volatile i64* dest, i64 val) {
#if defined(PD_WINDOWS)
    return InterlockedExchange64((volatile LONG64*)dest, val);
#else
#error Implement me!
#endif
}

i16 AtomicCompareAndSwap(volatile i16* dest, i16 newValue, i16 condition) {
#if defined(PD_WINDOWS)
    return InterlockedCompareExchange16((volatile SHORT*)dest, (SHORT)newValue, (SHORT)condition);
#else
#error Implement me!
#endif
}

i32 AtomicCompareAndSwap(volatile i32* dest, i32 newValue, i32 condition) {
#if defined(PD_WINDOWS)
    return InterlockedCompareExchange((volatile LONG*)dest, (LONG)newValue, (LONG)condition);
#else
#error Implement me!
#endif
}

i64 AtomicCompareAndSwap(volatile i64* dest, i64 newValue, i64 condition) {
#if defined(PD_WINDOWS)
    return InterlockedCompareExchange64((volatile LONG64*)dest, (LONG64)newValue, (LONG64)condition);
#else
#error Implement me!
#endif
}

}