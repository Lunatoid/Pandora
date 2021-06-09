#pragma once

namespace pd {

class Mutex {
public:
    Mutex();
    ~Mutex();

    /// <summary>
    /// Locks the mutex.
    /// </summary>

    /**
     * \brief Locks the mutex. Waits if the lock can't be acquired.
     */
    void Lock();

    /**
     * \brief Tries to lock the mutex. Does not wait if the lock can't be acquired.
     * 
     * \return True if the lock was acquired, false otherwise.
     */
    bool TryLock();

    /**
     * \brief Unlocks the mutex.
     */
    void Unlock();

    /**
     * \return The native mutex handle.
     */
    void* NativeHandle();

private:
    void* nativeData = nullptr;
};

}
