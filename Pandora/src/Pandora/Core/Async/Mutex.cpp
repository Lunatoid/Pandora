#include "Mutex.h"

#include "Pandora/Core/Data/Allocator.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

namespace pd {

Mutex::Mutex(int spinCount) {
#if defined(PD_WINDOWS)
    nativeData = New<CRITICAL_SECTION>();
    bool success = InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)nativeData, spinCount);
#else
#error Implement me!
#endif
}

Mutex::~Mutex() {
#if defined(PD_WINDOWS)
    DeleteCriticalSection((LPCRITICAL_SECTION)nativeData);
    Delete((LPCRITICAL_SECTION*)nativeData);
#else
#error Implement me!
#endif
}

void Mutex::Lock() {
#if defined(PD_WINDOWS)
    EnterCriticalSection((LPCRITICAL_SECTION)nativeData);
#else
#error Implement me!
#endif
}

bool Mutex::TryLock() {
#if defined(PD_WINDOWS)
    return TryEnterCriticalSection((LPCRITICAL_SECTION)nativeData);
#else
#error Implement me!
#endif
}

void Mutex::Unlock() {
#if defined(PD_WINDOWS)
    LeaveCriticalSection((LPCRITICAL_SECTION)nativeData);
#else
#error Implement me!
#endif
}

void* Mutex::NativeHandle() {
#if defined(PD_WINDOWS)
    return nativeData;
#else
#error Implement me!
#endif
}

}
