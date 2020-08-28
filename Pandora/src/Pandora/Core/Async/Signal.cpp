#include "Signal.h"

#include "Pandora/Core/Data/Allocator.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

namespace pd {

Signal::Signal() {
#if defined(PD_WINDOWS)
    nativeData = New<CONDITION_VARIABLE>();
    InitializeConditionVariable((PCONDITION_VARIABLE)nativeData);
#else
#error Implement me!
#endif
}

void Signal::Raise() {
#if defined(PD_WINDOWS)
    mutex.Lock();
    raised = true;
    mutex.Unlock();
    WakeConditionVariable((PCONDITION_VARIABLE)nativeData);
    Delete((PCONDITION_VARIABLE)nativeData);
#else
#error Implement me!
#endif
}

bool Signal::Wait(int timeoutMs) {
#if defined(PD_WINDOWS)
    bool timeOut = false;
    mutex.Lock();
    while (!raised) {
        int error = SleepConditionVariableCS((PCONDITION_VARIABLE)nativeData, (LPCRITICAL_SECTION)mutex.NativeHandle(), timeoutMs);

        if (error == 0 && GetLastError() == ERROR_TIMEOUT) {
            timeOut = true;
            break;
        }
    }

    if (!timeOut) {
        raised = false;
    }
    mutex.Unlock();

    return !timeOut;
#else
#error Implement me!
#endif
}

}
