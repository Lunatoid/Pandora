#pragma once

#include "Pandora/Core/Async/Mutex.h"

namespace pd {

class Signal {
public:
    Signal();

    /// <summary>
    /// Raises the signal.
    /// </summary>
    void Raise();

    /// <summary>
    /// Waits for the signal for the specified amount of time.
    /// </summary>
    /// <param name="timeoutMs">The timeout in milliseconds.</param>
    /// <returns><c>true</c> if it didn't time out, <c>false</c> if it did.</returns>
    bool Wait(int timeoutMs);

private:
    Mutex mutex;

#if defined(PD_WINDOWS)
    void* nativeData = nullptr;
    bool raised = false;
#endif
};

}
