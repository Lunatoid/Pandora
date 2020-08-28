#pragma once

namespace pd {

class Mutex {
public:
    Mutex(int spinCount = 32);
    ~Mutex();

    /// <summary>
    /// Locks the mutex.
    /// </summary>
    void Lock();

    /// <summary>
    /// Tries to Lock the mutex.
    /// </summary>
    /// <returns>Whether or not it locked successfully</returns>
    bool TryLock();

    /// <summary>
    /// Unlocks the mutex.
    /// </summary>
    void Unlock();

    /// <summary>
    /// Returns the native mutex handle.
    /// On Windows this is the <c>CRITICAL_SECTION</c>.
    /// </summary>
    /// <returns>The native mutex handle.</returns>
    void* NativeHandle();

private:
    void* nativeData = nullptr;
};

}
